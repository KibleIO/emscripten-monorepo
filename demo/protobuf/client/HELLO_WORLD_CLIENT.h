/*

Dear future reader, this file is built to serve as a sample "generated"
protobuf based HTTP RPC client. In the future perhaps we will create a way
for this to *actually* be automatically generated. Until then, please follow
this exact format when interacting with any HTTP RPC servers in emscripten.

*/

#ifndef _HELLO_WORLD_CLIENT_H_
#define _HELLO_WORLD_CLIENT_H_

#include <google/protobuf/util/json_util.h>
#include <string>
#include "../gen/test.pb.h"
#include "HTTP.h"
#include "HTTP_HELPER.h"

//Please put in the service package, as defined in the protobuf file here
#define HELLO_WORLD_CLIENT_PACKAGE "gaia"

//Please put in the service name here
#define HELLO_WORLD_CLIENT_SERVICE_NAME "HelloWorld"

//Please add the http path as defined in twirp
#define HELLO_WORLD_CLIENT_HTTP_PATH "/kible"

struct HELLO_WORLD_CLIENT {
	std::string service_address;
};

bool Initialize_HELLO_WORLD_CLIENT(HELLO_WORLD_CLIENT*, std::string);
void Delete_HELLO_WORLD_CLIENT(HELLO_WORLD_CLIENT*);

//Please put a string representation of each endpoint here
#define HELLO_WORLD_CLIENT_HELLO "Hello"
bool Hello_HELLO_WORLD_CLIENT(HELLO_WORLD_CLIENT*, gaia::HelloReq*,
	gaia::HelloResp*);

#endif