# umbelt-sw

## How to Run

Prerequisites: PlatformIO

1. Clone repository
2. Pull in submodules with `git submodule init` and `git submodule update`.
    (More on working with submodules [here](https://git-scm.com/book/en/v2/Git-Tools-Submodules).)
3. Clone [the Umbelt BSP package](https://github.com/cygann/Adafruit_nRF52_Arduino) as a sibling directory to `umbelt-sw`. 
4. Run `pio run` in the `umbelt-sw` directory 
5. Run `python variant_pre_build.py` to copy the BSP files such that PIO can use them.
6. Compile with `pio run`.
7. Upload (and compile) with `pio run -t upload`.
8. Monitor serial activity with `pio device monitor`.
