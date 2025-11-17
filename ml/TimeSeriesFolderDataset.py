"""saves all data in csv files as tensors"""

import os
import numpy as np
import pandas as pd
import torch
from torch.utils.data import Dataset

class TimeSeriesFolderDataset(Dataset):
    def __init__(self, root_dir, fixed_len=None):
        """
        root_dir: path like 'data'
        Assumes: root_dir/class_name/*.csv
        """
        self.root_dir = root_dir

        self.class_names = sorted(
            [d for d in os.listdir(root_dir) if os.path.isdir(os.path.join(root_dir, d))]
        )
        self.class_to_idx = {cls_name: i for i, cls_name in enumerate(self.class_names)}

        self.samples = []
        lengths = []

        for cls_name in self.class_names:
            cls_dir = os.path.join(root_dir, cls_name)
            for fname in os.listdir(cls_dir):
                if fname.lower().endswith(".csv"):
                    path = os.path.join(cls_dir, fname)
                    label = self.class_to_idx[cls_name]
                    self.samples.append((path, label))

                    df_tmp = pd.read_csv(path)
                    lengths.append(len(df_tmp))

        if not self.samples:
            raise RuntimeError(f"No CSV files found in {root_dir}")

        if fixed_len is None:
            self.seq_len = min(lengths)   # safest: crop to shortest
        else:
            self.seq_len = fixed_len

        print(f"Found {len(self.samples)} files across {len(self.class_names)} classes:")
        for cls, idx in self.class_to_idx.items():
            print(f"  {idx}: {cls}")
        print(f"Using fixed sequence length: {self.seq_len}")

    def __len__(self):
        return len(self.samples)

    def __getitem__(self, idx):
        path, label = self.samples[idx]

        df = pd.read_csv(path)                    # (L, 3)
        arr = df.values.astype(np.float32)        # numpy (L, 3)

        L, C = arr.shape
        if L >= self.seq_len:
            arr = arr[:self.seq_len, :]
        else:
            pad_len = self.seq_len - L
            pad = np.zeros((pad_len, C), dtype=np.float32)
            arr = np.vstack([arr, pad])          # (seq_len, 3)

        x = torch.from_numpy(arr).permute(1, 0)  # (3, seq_len)
        y = torch.tensor(label, dtype=torch.long)
        return x, y
