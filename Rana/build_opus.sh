#!/bin/bash

rm -rf opus/

wget https://github.com/xiph/opus/archive/refs/tags/v1.3.1.tar.gz

tar -xzf v1.3.1.tar.gz
rm v1.3.1.tar.gz
mv opus-1.3.1 opus

cd opus
mkdir build
cd build

../autogen.sh
emconfigure ../configure --disable-extra-programs --disable-rtcd --disable-intrinsics
emmake make