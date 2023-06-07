build_docker_arm:
	docker build --platform linux/arm64 --file Dockerfile -t kible/proto:arm .
	docker login -u "kible" -p "eehKgVR4QmoED8" docker.io
	docker push kible/proto:arm

build_docker_x86:
	docker build --platform linux/amd64 --file Dockerfile -t kible/proto:x86 .
	docker login -u "kible" -p "eehKgVR4QmoED8" docker.io
	docker push kible/proto:x86

install_proto_arm:
	docker run --platform linux/arm64 --pull=always --rm -it -v `pwd`:/root kible/proto:arm ./install_protolib.sh

install_proto_x86:
	docker run --platform linux/amd64 --pull=always --rm -it -v `pwd`:/root/working kible/proto:x86 ./install_protolib.sh

build-proto-arm:
	docker run --platform linux/arm64 --pull=always --rm -it -v `pwd`:/root kible/proto:arm make build_proto

build-proto-x86:
	docker run --platform linux/amd64 --pull=always --rm -it -v `pwd`:/root/working kible/proto:x86 make build_proto

build_opus-x86:
	docker run --platform linux/amd64 --pull=always --rm -it -v `pwd`:/root/working kible/proto:x86 make build_opus

build_broadway-x86:
	docker run --platform linux/amd64 --pull=always --rm -it -v `pwd`:/root/working kible/proto:x86 make build_broadway

build_proto:
	mkdir -p src/kible.io/gen
	protoc --proto_path=proto-monorepo/idl/kible.io --cpp_out=./src/kible.io/gen proto-monorepo/idl/kible.io/*/*.proto
	rename 's/\.cc$$/.cpp/' src/kible.io/gen/*/*.cc

build_broadway:
	make -C src/kible.io/Broadway-H.264-decoder

build_opus:
	sh build_opus.sh

clean_all:
	rm -rf src/kible.io/gen