#!/bin/bash

mkdir -p build
cd build

BUILD_TESTS=OFF

if [ $# -gt 0 ]; then
    BUILD_TESTS=$1
fi

CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=$BUILD_TESTS ..

make

if [ -e ./tests ]; then
    make test
fi