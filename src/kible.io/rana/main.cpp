#include "RANA_EXT.h"
#include <Utilities/ASSERT.h>
#include <Utilities/UTILS.h>
#include "LIMITS.h"
#include "client/EDGE_CLIENT.h"
#include "utils/UTILS.h"

void Callback_Launch_Themis_Client(google::protobuf::Message *message,
	void *user_data) {
	
	KCONTEXT *ctx = (KCONTEXT*) user_data;
	RANA_EXT *rana_ext = new RANA_EXT;
	
	if (message == NULL) {
		Show_Error_Message(std::string("Failed to communicate with Themis"));
		std::cout << "request failed" << std::endl;
	} else {
		kible::themis::LaunchResponse response;
		response.CopyFrom(*message);

		ASSERT_E_B((Initialize_RANA_EXT(rana_ext, ctx)),
		"failed to initialize rana_ext", ctx);
	}
}

void Callback_Themis_Edge_Client(google::protobuf::Message *message,
	void *user_data) {
	
	KCONTEXT *ctx = (KCONTEXT*) user_data;
	RANA_EXT *rana_ext = new RANA_EXT;
	std::string themis_url;

	if (message == NULL) {
		Show_Error_Message(std::string("Failed to communicate with Edge"));
		std::cout << "request failed" << std::endl;
	} else {
		kible::edge::ThemisResponse response;
		response.CopyFrom(*message);

		themis_url = response.address();

		std::string delimiter = "alienhub.xyz";
		ctx->themis_url = themis_url.substr(0,
			themis_url.find(delimiter)) + ctx->url +
			themis_url.substr(themis_url.find(delimiter) +
			delimiter.length());

		Launch_THEMIS_CLIENT(ctx, Callback_Launch_Themis_Client,
			(void*) ctx);
	}
}

int main() {
	KCONTEXT *ctx = new KCONTEXT;

	#ifdef TESTING_BUILD

	Initialize_KCONTEXT(ctx, __CORE_SYSTEM__, false);

	#else

	Initialize_KCONTEXT(ctx, __CORE_SYSTEM__, true);

	#endif

	LOGGER_INFO(ctx, {
		{"message", "main begun"},
		{"version", SOFTWARE_VERSION},
	});

	ctx->core_services_backbone = ROOT_SOCKET_TYPE_WS;

	#ifdef TESTING_BUILD

	ctx->core_services_backbone_port = THEMIS_PORT;
	ctx->http_services_backbone_port = THEMIS_PORT_HTTP;

	ctx->themis_url = "localhost";

	Launch_THEMIS_CLIENT(ctx, Callback_Launch_Themis_Client, (void*) ctx);

	#else

	ctx->core_services_backbone_port = 443;
	ctx->http_services_backbone_port = 443;
	Themis_EDGE_CLIENT(std::string("https://") + ctx->url, ctx->uuid,
		Callback_Themis_Edge_Client, (void*) ctx);

	#endif

	return 0;
}
