from debian:11

SHELL ["/bin/bash", "--login", "-c"]
RUN export DEBIAN_FRONTEND=noninteractive
RUN apt update
RUN apt install -y git cmake make gcc g++ wget python3 autoconf libtool curl unzip rename xz-utils lbzip2
WORKDIR /root
RUN git clone https://github.com/emscripten-core/emsdk.git
RUN git clone https://github.com/KibleIO/protobuf-emscripten.git
RUN wget https://github.com/protocolbuffers/protobuf/releases/download/v3.1.0/protobuf-cpp-3.1.0.tar.gz
RUN tar -xvf protobuf-cpp-3.1.0.tar.gz 
WORKDIR /root/emsdk
RUN ./emsdk install latest
RUN ./emsdk activate latest
RUN echo "source /root/emsdk/emsdk_env.sh" >> /root/.bashrc
WORKDIR /root/protobuf-3.1.0
RUN ./configure --prefix=/usr
RUN make
RUN make install
WORKDIR /root/protobuf-emscripten/3.1.0
RUN sh autogen.sh
RUN emconfigure ./configure
RUN emmake make
RUN make install
WORKDIR /root
RUN rm -rf *