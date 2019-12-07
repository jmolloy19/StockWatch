#!/bin/bash
set -eu
rm -rf build || true
mkdir build
cd build
cmake .. #cmake -D CMAKE_BUILD_TYPE=DEBUG
make -j
cd -
