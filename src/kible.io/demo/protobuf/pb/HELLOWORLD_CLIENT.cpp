#include "HELLOWORLD_CLIENT.h"

HTTP_Protobuf_Init_Delete(gaia, HELLOWORLD, /kible)

HTTP_Protobuf_Endpoint_Definition(HELLOWORLD, Hello, gaia::HelloReq,
	gaia::HelloResp)