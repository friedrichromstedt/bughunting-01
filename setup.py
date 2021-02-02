# Developed since: Feb 2021

from setuptools import setup, Extension

modelmodule = Extension(
    'bughuntingfr.model',
    sources=[
        'lib/bughuntingfr/model/model.cpp',
        'lib/bughuntingfr/model/image.cpp',
    ],
    depends=[
        'lib/bughuntingfr/model/image.h',
    ],
)

setup(
    name='bughuntingfr',
    version='0.1.0',
    ext_modules=[modelmodule],
    package_dir={'': 'lib'},
    packages=['bughuntingfr'],
)
