'''Copies the umbelt variant.h and variant.cpp into platformio's Adafruit nRF52
framework.'''

import os
import shutil

HOME = os.path.expanduser('~') + '/'

# variant files are copied from this locally-cloned repo:
DEFAULT_UMBELT_NRF_FRAMEWORK_PATH = '../Adafruit_nRF52_Arduino/'
# to the platformio install here:
DEFAULT_PIO_NRF_FRAMEWORK_PATH = (HOME
                    + '.platformio/packages/framework-arduinoadafruitnrf52/')

UMBELT_VARIANT_FILE_PATH = 'variants/feather_nrf52840_sense/'
VARIANT_FILES = ['variant.cpp', 'variant.h']


def get_umbelt_variant_files():
    # find repo
    # TODO: user override of default location through environment or cmd arg
    if not os.path.exists(DEFAULT_UMBELT_NRF_FRAMEWORK_PATH):
        raise FileNotFoundError('Cannot find Adafruit_nRF52_Arduino repo at '
            + DEFAULT_UMBELT_NRF_FRAMEWORK_PATH
            + '\nPlease clone https://github.com/cygann/Adafruit_nRF52_Arduino'
            + '\ninto the same directory that contains this umbelt-sw repo.')
    repo_path = DEFAULT_UMBELT_NRF_FRAMEWORK_PATH

    # find variant files
    file_paths = []
    for variant_file in VARIANT_FILES:
        file_path = repo_path + UMBELT_VARIANT_FILE_PATH + variant_file
        if not os.path.exists(file_path):
            raise FileNotFoundError('Cannot find Umbelt variant file '
                                    + file_path)
        file_paths.append(file_path)
    return file_paths


def get_pio_framework_variant_dir():
    # find platformio framework directory
    if not os.path.exists(DEFAULT_PIO_NRF_FRAMEWORK_PATH):
        raise FileNotFoundError('Cannot find framework-arduinoadafruitnrf52 '
                                + 'repo at ' + DEFAULT_PIO_NRF_FRAMEWORK_PATH)
    pio_framework_path = DEFAULT_PIO_NRF_FRAMEWORK_PATH

    # check that feather sense variant directory exists
    pio_framework_variant_dir = pio_framework_path + UMBELT_VARIANT_FILE_PATH
    if not os.path.exists(pio_framework_variant_dir):
        raise FileNotFoundError('Cannot find platformio framework variant '
                                + 'directory at ' + pio_framework_variant_dir)
    return pio_framework_variant_dir


def copy_variant_files(source_files, destination_dir):
    for source in source_files:
        shutil.copy(source, destination_dir)
        print('Copied ' + source + ' to ' + destination_dir)
    print('Done copying Umbelt variant files to platformio framework')


def main():
    print()
    source_files = get_umbelt_variant_files()
    desination_dir = get_pio_framework_variant_dir()
    copy_variant_files(source_files, desination_dir)
    print()


main()
