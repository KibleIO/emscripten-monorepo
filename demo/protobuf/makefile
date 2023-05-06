build_proto_arm:
	docker build --platform linux/arm64 --file Dockerfile -t kible/proto:arm .
	docker login -u "kible" -p "eehKgVR4QmoED8" docker.io
	docker push ible/proto:arm

build_proto_x86:
	docker build --platform linux/amd64 --file Dockerfile -t kible/proto:x86 .
	docker login -u "kible" -p "eehKgVR4QmoED8" docker.io
	docker push ible/proto:x86