# ml/model.py

import os
import torch
import torch.nn as nn
from torch.utils.data import DataLoader, random_split
from pytorch_tcn import TCN
from TimeSeriesFolderDataset import TimeSeriesFolderDataset

root_dir    = "data"        # your data root
num_classes = 5             # air pinch, full fist, open hand, rest, true rest
batch_size  = 16
num_epochs  = 20
lr          = 1e-3
val_ratio   = 0.2

os.makedirs("ml/checkpoints", exist_ok=True)
torch.manual_seed(42)

full_dataset = TimeSeriesFolderDataset(root_dir) 

n_total = len(full_dataset)
n_val   = int(n_total * val_ratio)
n_train = n_total - n_val

train_dataset, val_dataset = random_split(full_dataset, [n_train, n_val])

train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
val_loader   = DataLoader(val_dataset,   batch_size=batch_size, shuffle=False)

x_batch, y_batch = next(iter(train_loader))
print("Batch x shape:", x_batch.shape)  # (B, 3, seq_len)
print("Batch y shape:", y_batch.shape)  # (B,)


model = TCN(
    num_inputs = 3, # 3 sensors
    num_channels = [32, 32, 64],
    kernel_size = 4, # default
    dilations = None, # default
    dilation_reset = None, # default
    dropout = 0.1, # default
    causal = True, # default
    use_norm = 'weight_norm', # default
    activation = 'relu', # default
    kernel_initializer = 'xavier_uniform', # default, bai et al. paper had normal initialization
    use_skip_connections = False, # default
    input_shape = 'NCL', # default
    embedding_shapes = None, # default
    embedding_mode = 'add', # default
    use_gate = False, # default, true -> maybe better, definitely computationally costlier
    lookahead = 0, # default, deprecated
    output_projection = num_classes, # default
    output_activation= None # defaults to None
)




device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model.to(device)

print(model)
num_params = sum(p.numel() for p in model.parameters())
print(f"Number of trainable parameters: {num_params}")

# ---------- Loss & Optimizer ----------
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=lr)

# ---------- Training Loop ----------
best_val_acc = 0.0

for epoch in range(num_epochs):
    # ---- Train ----
    model.train()
    train_loss = 0.0
    correct = 0
    total = 0

    for x_batch, y_batch in train_loader:
        x_batch = x_batch.to(device)  # (B, 3, L)
        y_batch = y_batch.to(device)  # (B,)

        optimizer.zero_grad()

        # TCN outputs (B, num_classes, L)
        logits_seq = model(x_batch)

        # Pool over time dimension to get one prediction per sequence
        logits = logits_seq.mean(dim=-1)  # (B, num_classes)
        # or: logits = logits_seq[:, :, -1]

        loss = criterion(logits, y_batch)
        loss.backward()
        optimizer.step()

        train_loss += loss.item() * x_batch.size(0)
        preds = logits.argmax(dim=1)
        correct += (preds == y_batch).sum().item()
        total += y_batch.size(0)

    train_loss /= total
    train_acc = correct / total

    # ---- Validation ----
    model.eval()
    val_loss = 0.0
    val_correct = 0
    val_total = 0

    with torch.no_grad():
        for x_batch, y_batch in val_loader:
            x_batch = x_batch.to(device)
            y_batch = y_batch.to(device)

            logits_seq = model(x_batch)        # (B, num_classes, L)
            logits = logits_seq.mean(dim=-1)   # (B, num_classes)

            loss = criterion(logits, y_batch)

            val_loss += loss.item() * x_batch.size(0)
            preds = logits.argmax(dim=1)
            val_correct += (preds == y_batch).sum().item()
            val_total += y_batch.size(0)

    val_loss /= val_total
    val_acc = val_correct / val_total

    print(
        f"Epoch {epoch+1}/{num_epochs} "
        f"Train loss: {train_loss:.4f}, acc: {train_acc:.3f} | "
        f"Val loss: {val_loss:.4f}, acc: {val_acc:.3f}"
    )

    # ---- Checkpoint best model ----
    if val_acc > best_val_acc:
        best_val_acc = val_acc
        torch.save(model.state_dict(), "ml/checkpoints/best_model.pt")
        print(f"Saved new best model (val_acc={val_acc:.3f})")

print("training finished wjiogaioeojaie best val_acc:", best_val_acc)
