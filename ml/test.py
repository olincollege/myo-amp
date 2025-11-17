import torch
import pandas as pd
from pytorch_tcn import TCN
from TimeSeriesFolderDataset import TimeSeriesFolderDataset


def test_one_csv(path):
    root_dir = "data"
    ds = TimeSeriesFolderDataset(root_dir)  # to get seq_len + class_names

    num_classes = len(ds.class_names)
    seq_len = ds.seq_len

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



    model.load_state_dict(torch.load("ml/checkpoints/best_model.pt", map_location="cpu"))
    model.eval()
 

    df = pd.read_csv(path)
    arr = df.values.astype("float32")
    L, C = arr.shape
    if L >= seq_len:
        arr = arr[:seq_len]
    else:
        import numpy as np
        pad = np.zeros((seq_len - L, C), dtype="float32")
        arr = np.vstack([arr, pad])

    x = torch.from_numpy(arr).permute(1, 0).unsqueeze(0)  # (1,3,L)

    with torch.no_grad():
        logits_seq = model(x)
        logits = logits_seq.mean(dim=-1)  # (1, num_classes)
        probs = torch.softmax(logits, dim=-1)
        pred_idx = probs.argmax(dim=-1).item()

    return pred_idx, ds.class_names
