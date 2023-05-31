#ifndef _PB_EDGE_CLIENT_H_
#define _PB_EDGE_CLIENT_H_

#include "../../gen/edge/edge.pb.h"
#include "HTTP_HELPER.h"

HTTP_Protobuf_Begin(EDGE)

HTTP_Protobuf_Endpoint(EDGE, Themis, kible::edge::ThemisRequest, kible::edge::ThemisResponse)
HTTP_Protobuf_Endpoint(EDGE, Kasm, kible::edge::KasmRequest, kible::edge::KasmResponse)
HTTP_Protobuf_Endpoint(EDGE, Login, kible::edge::LoginRequest, kible::edge::LoginResponse)

HTTP_Protobuf_End()

#endif