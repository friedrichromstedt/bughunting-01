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

singlemodule = Extension(
    'bughuntingfrmod',
    sources=['lib/bughuntingfrmod/bughuntingfrmod.cpp'],
)

setup(
    name='bughuntingfr',
    version='0.1.0',
    ext_modules=[
        modelmodule,
        singlemodule,
    ],
    package_dir={'': 'lib'},
    packages=['bughuntingfr'],
)
