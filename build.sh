#!/bin/bash
set -eu
rm -rf build || true
mkdir build

pushd src
find ./ -iname *.h -o -iname *.cc | xargs clang-format -i -style=file
popd

pushd build
cmake ..
make -j6
popd

