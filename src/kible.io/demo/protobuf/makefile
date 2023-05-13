CC = em++ -O3
EXEC = index.html
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

CXXFLAGS = -pthread -sUSE_PTHREADS -sASYNCIFY -sFETCH protolib/libprotobuf.a
CC_BUILD_FLAGS = -I./protolib -pthread -sUSE_PTHREADS

default: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(CC_COMPILE_FLAGS) $(CXXFLAGS)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(CC_COMPILE_FLAGS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_BUILD_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

build_proto_arm:
	docker build --platform linux/arm64 --file Dockerfile -t kible/proto:arm .
	docker login -u "kible" -p "eehKgVR4QmoED8" docker.io
	docker push kible/proto:arm

build_proto_x86:
	docker build --platform linux/amd64 --file Dockerfile -t kible/proto:x86 .
	docker login -u "kible" -p "eehKgVR4QmoED8" docker.io
	docker push kible/proto:x86

install_proto_arm:
	docker run --platform linux/arm64 --pull=always --rm -it -v `pwd`:/root kible/proto:arm ./install.sh

install_proto_x86:
	docker run --platform linux/amd64 --pull=always --rm -it -v `pwd`:/root kible/proto:x86 ./install.sh

build-proto-arm:
	docker run --platform linux/arm64 --pull=always --rm -it -v `pwd`:/root kible/proto:arm make build_proto

build-proto-x86:
	docker run --platform linux/amd64 --pull=always --rm -it -v `pwd`:/root kible/proto:x86 make build_proto

build_proto:
	mkdir -p gen
	protoc -I ./ --cpp_out=./gen *.proto
	rename 's/\.cc$$/.cpp/' ./gen/*.cc

clean_all:
	rm -rf gen protolib