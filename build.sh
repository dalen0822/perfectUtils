#!/bin/sh

cd build
rm -rf *
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ../utils/data_buf
make

