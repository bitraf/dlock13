#!/bin/bash -xe

mkdir -p build

if [[ $MsgFlo_DIR == "" ]]
then
    mkdir -p thirdparty/msgflo-cpp/build
    cd thirdparty/msgflo-cpp/build
    cmake -DCMAKE_INSTALL_PREFIX=../../../build/install ..
    make -j4
    make install
    cd ../../..
    MsgFlo_DIR=build/install/lib/cmake/MsgFlo
else
    echo "Using MsgFlo installation from $MsgFlo_DIR"
fi

cd build
cmake -DMsgFlo_DIR=$MsgFlo_DIR ..
make -j4
