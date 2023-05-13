#!/bin/bash

rm -rf src/kible.io/opus/

wget https://downloads.xiph.org/releases/opus/opus-1.3.tar.gz

tar -xzf opus-1.3.tar.gz
rm opus-1.3.tar.gz
mv opus-1.3 src/kible.io/opus/

cd src/kible.io/opus/

emconfigure ./configure --disable-extra-programs --disable-rtcd --disable-intrinsics
emmake make