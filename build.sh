#!/bin/bash
set -eu
rm -rf build || true
mkdir build
cd build
cmake ..
make -j
cd -
