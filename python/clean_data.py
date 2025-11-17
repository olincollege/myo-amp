from pathlib import Path
import pandas as pd

folderpath = Path("data/")

for file in folderpath.glob("*.csv"):
    df = pd.read_csv(file, engine="python")

    df = df.applymap(
        lambda x: str(x).strip() if isinstance(x, str) else x
    )

    df.to_csv(file, index=False)

print("Done.")

