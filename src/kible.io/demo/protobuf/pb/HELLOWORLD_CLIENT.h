/*

Dear future reader, this file is built to serve as a sample "generated"
protobuf based HTTP RPC client. In the future perhaps we will create a way
for this to *actually* be automatically generated. Until then, please follow
this exact format when interacting with any HTTP RPC servers in emscripten.

*/

#ifndef _PB_HELLOWORLD_CLIENT_H_
#define _PB_HELLOWORLD_CLIENT_H_

#include "../gen/test.pb.h"
#include "HTTP_HELPER.h"

/*

Things created by HTTP_Protobuf_Begin(HELLOWORLD)

pb::HELLOWORLD_CLIENT
bool Initialize_HELLOWORLD_CLIENT(pb::HELLOWORLD_CLIENT*, std::string)
void Delete_HELLOWORLD_CLIENT(pb::HELLOWORLD_CLIENT*)

*/

HTTP_Protobuf_Begin(HELLOWORLD)

/*

Things created by HTTP_Protobuf_Endpoint(HELLOWORLD, Hello, gaia::HelloReq,
	gaia::HelloResp)

bool Hello_HELLOWORLD_CLIENT(pb::HELLOWORLD_CLIENT*, gaia::HelloReq*,
	gaia::HelloResp*)

*/

HTTP_Protobuf_Endpoint(HELLOWORLD, Hello, gaia::HelloReq, gaia::HelloResp)

HTTP_Protobuf_End()

#endif