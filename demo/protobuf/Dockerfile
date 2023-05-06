from debian:11

SHELL ["/bin/bash", "-c"]
RUN export DEBIAN_FRONTEND=noninteractive
RUN apt update
RUN apt install -y git cmake make gcc g++ wget python autoconf libtool curl unzip
WORKDIR /root
RUN git clone https://github.com/emscripten-core/emsdk.git
RUN git clone https://github.com/invokr/protobuf-emscripten.git
WORKDIR /root/emsdk
RUN ./emsdk install emscripten-1.36.14
RUN ./emsdk activate emscripten-1.36.14
RUN source ./emsdk_env.sh
WORKDIR /root/protobuf-emscripten/3.1.0
RUN sh autogen.sh
RUN emconfigure --em-config LLVM_ROOT=/usr/bin; ./configure
RUN emmake --em-config LLVM_ROOT=/usr/bin; make