#!/bin/bash
set -eu
rm -rf build || true
mkdir build
cd build
if [ "${1-default_no_tests}" == "test" ]; then
    cmake .. -DTest:BOOL=ON
else
    cmake ..
fi
make -j
cd -
