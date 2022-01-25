# umbelt-sw

## How to Run

Prerequisites: PlatformIO (PIO)

First time setup:

1. Clone repository
2. Pull in submodules with `git submodule init` and `git submodule update`.
    (More on working with submodules [here](https://git-scm.com/book/en/v2/Git-Tools-Submodules).)
3. Clone [the Umbelt board support package (BSP)](https://github.com/cygann/Adafruit_nRF52_Arduino) as a sibling directory to `umbelt-sw`. This is a modified version of the BSP for the [Adafruit Feather Sense](https://www.adafruit.com/product/4516).
4. Run `pio run` in the `umbelt-sw` directory. This will download the Feather Sense BSP from PlatformIO's servers to PlatformIO's BSP install location, then fail to compile the Umbelt code because it's using this BSP instead of the Umbelt one.
5. Run `python variant_pre_build.py` to overwrite the Feather Sense BSP with the Umbelt BSP. If PIO ever reverts to the original config, which will manifest as the same compilation errors as in the previous step, just re-run this script.

Compiling, uploading to device, and monitoring serial output:

6. Compile with `pio run`.
7. Upload (and compile) with `pio run -t upload`.
8. Monitor serial activity with `pio device monitor`.
