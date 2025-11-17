import matplotlib
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.metrics import confusion_matrix, classification_report
from TimeSeriesFolderDataset import TimeSeriesFolderDataset
from test import test_one_csv
from test import test_one_csv
import os

path = "data/open_hand/open_hand.csv"

def main():
    root_dir = "data"

    # Use the dataset to get file paths + true labels
    ds = TimeSeriesFolderDataset(root_dir)
    class_names = ds.class_names
    num_classes = len(class_names)

    print("Classes:", class_names)
    print("Total samples:", len(ds.samples))

    y_true = []
    y_pred = []

    # Loop over every CSV file
    for csv_path, true_label_idx in ds.samples:
        pred_idx, _ = test_one_csv(csv_path)  # returns (pred_idx, class_names)
        y_true.append(true_label_idx)
        y_pred.append(pred_idx)

    # Compute confusion matrix
    cm = confusion_matrix(y_true, y_pred, labels=list(range(num_classes)))
 
    # Plot confusion matrix
    plt.figure(figsize=(8, 6))
    # cmap = matplotlib.colors.LinearSegmentedColormap.from_list('PinkPurple', ['pink', 'purple'])
    sns.heatmap(
        cm,
        annot=True,
        fmt="d",
        xticklabels=class_names,
        yticklabels=class_names,
        cmap="Purples",
    )
    plt.xlabel("Predicted")
    plt.ylabel("True")
    plt.title("Confusion Matrix (All CSV Files)")
    plt.tight_layout()

    os.makedirs("plots", exist_ok=True)
    plt.savefig("plots/confusion_matrix.png")
    plt.show()


if __name__ == "__main__":
    main()
