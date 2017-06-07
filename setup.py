import os
from setuptools import find_packages, setup

with open(os.path.join(os.path.dirname(__file__), 'README.md')) as readme:
    README = readme.read()

# allow setup.py to be run from any path
os.chdir(os.path.normpath(os.path.join(os.path.abspath(__file__), os.pardir)))

setup(
    name='dlock13',
    version='0.4.0',
    packages=find_packages(),
    include_package_data=True,
    license='MIT License',
    description='Python API for MQTT-based doorlocks',
    long_description=README,
    url='https://github.com/bitraf/dlock13',
    author='Jon Nordby',
    author_email='jononor@gmail.com',
    classifiers=[
        'Environment :: Web Environment',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        # Replace these appropriately if you are stuck on Python 2.
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 2.6',
        'Topic :: Internet :: WWW/HTTP',
    ],
    install_requires=[
        "paho-mqtt >= 1.1",
    ],
    scripts=[
        'dlock13-open'
    ],
)
