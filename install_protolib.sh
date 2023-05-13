#!/bin/bash

rm -rf src/kible.io/protolib
mkdir -p src/kible.io/protolib
cp -r /usr/local/include/google src/kible.io/protolib
cp /usr/local/lib/libprotobuf.a src/kible.io/protolib