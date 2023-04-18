#!/bin/bash

rm -rf opus/
git clone https://github.com/xiph/opus.git
cd opus
mkdir build
cd build
../autogen.sh
emconfigure ../configure --disable-extra-programs --disable-rtcd --disable-intrinsics
emmake make