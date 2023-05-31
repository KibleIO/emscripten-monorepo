#include "THEMIS_CLIENT.h"

HTTP_Protobuf_Init_Delete(kible.themis, THEMIS, /api)

HTTP_Protobuf_Endpoint_Definition(THEMIS, Check, kible::themis::CheckRequest,
	kible::themis::CheckResponse)
HTTP_Protobuf_Endpoint_Definition(THEMIS, Dimensions, kible::themis::DimensionsRequest,
	kible::themis::DimensionsResponse)
HTTP_Protobuf_Endpoint_Definition(THEMIS, Launch, kible::themis::LaunchRequest,
	kible::themis::LaunchResponse)
HTTP_Protobuf_Endpoint_Definition(THEMIS, Update, kible::themis::UpdateRequest,
	kible::themis::UpdateResponse)
HTTP_Protobuf_Endpoint_Definition(THEMIS, Density, kible::themis::DensityRequest,
	kible::themis::DensityResponse)
HTTP_Protobuf_Endpoint_Definition(THEMIS, Ping, kible::themis::PingRequest,
	kible::themis::PingResponse)
HTTP_Protobuf_Endpoint_Definition(THEMIS, FPS, kible::themis::FPSRequest,
	kible::themis::FPSResponse)