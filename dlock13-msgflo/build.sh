#!/bin/bash

git submodule update --init --recursive

mkdir -p build

if [[ $MsgFlo_DIR == "" ]]
then
    mkdir -p thirdparty/msgflo-cpp/build
    cd thirdparty/msgflo-cpp/build
    cmake -DCMAKE_INSTALL_PREFIX=../../../build/msgflo-cpp-install ..
    make -j
    make install
    cd ../../..
    MsgFlo_DIR=build/msgflo-cpp-install/lib/cmake/MsgFlo
else
    echo "Using MsgFlo installation from $MsgFlo_DIR"
fi

cd build
cmake -DMsgFlo_DIR=$MsgFlo_DIR ..
make
