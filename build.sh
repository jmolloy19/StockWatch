#!/bin/bash
set -eu
rm -rf Build || true
mkdir Build
cd Build
cmake ..
make -j
cd -
