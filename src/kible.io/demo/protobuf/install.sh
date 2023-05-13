#!/bin/bash

rm -rf ./protolib
mkdir -p ./protolib
cp -r /usr/local/include/google ./protolib
cp /usr/local/lib/libprotobuf.a ./protolib