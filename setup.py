import os
from setuptools import find_packages
from distutils.core import setup, Extension
from distutils.sysconfig import get_python_lib

sraam_sources = ['./missile/pySRAAM.cpp']
for filename in os.listdir("./missile/src"):
    name_split = filename.split('.')
    if len(name_split) > 1 and name_split[1] in ['cpp', 'hpp', 'h']:
        sraam_sources.append(os.path.join("./missile/src", filename))

setup(
    name="pyaircombat",
    version="1.1",
    author="we",
    author_email="xiwei1999@qq.com",
    description="python air combat env base on JSBSIM",
    python_requires=">=3.7",
    packages=find_packages(),
    package_data={
        "pyaircombat.plane": [
            "jsbsim-1.1.12/aircraft/*/*.xml",
            "jsbsim-1.1.12/aircraft/*/INSTALL",
            "jsbsim-1.1.12/aircraft/*/*/*.xml",
            "jsbsim-1.1.12/systems/*.xml",
            "jsbsim-1.1.12/engine/*.xml",
        ]
    },
    install_requires=['numpy', 'matplotlib', 'gym', 'jsbsim==1.1.12'],
    ext_modules=[
        Extension("SRAAM",
                  include_dirs=["./missile/include/", os.path.join(get_python_lib(), 'numpy', 'core', 'include')],
                  sources=sraam_sources)],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
    ]
)
