"""
Casey Walker (2024)
24-673 Soft Robotics
Carnegie Mellon University
--------------------------------------------------------------------------------
"""

from pathlib import Path

import numpy as np
from matplotlib import pyplot as pl


project_root_dirpath = Path("../")

def main():
    calibration_data_dirpath = project_root_dirpath/"data/calibration/"
    if not calibration_data_dirpath.exists():
        return

    filepaths = sorted(
        filepath for filepath in calibration_data_dirpath.iterdir()
        if (filepath.is_file() and filepath.suffix == ".csv")
    )

    for filepath in filepaths:
        calibration_data = np.loadtxt(filepath, delimiter=",")
        t = calibration_data[:, 0]
        s1 = calibration_data[:, 1]
        s2 = calibration_data[:, 2]

        fig = pl.figure()
        ax = fig.add_subplot()

        ax.plot(t, s1, t, s2)
        ax.set_title(str(filepath))

    pl.show()


if __name__ == "__main__":
    main()
