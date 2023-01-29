from setuptools import setup
import site
import sys

site.ENABLE_USER_SITE = "--user" in sys.argv[1:]

setup(
    name='umbelt',
    version='0.0.1',
    description='Client library for using Umbelt boards',
    url='http://github.com/cygann/umbelt-sw',
    author='Natalie Cygan',
    license='MIT',
    zip_safe=False,
    install_requires=[
        "adafruit-circuitpython-ble"
    ]
)
