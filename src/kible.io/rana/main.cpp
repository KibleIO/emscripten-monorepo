#include "RANA_EXT.h"
#include <Utilities/ASSERT.h>
#include <Utilities/UTILS.h>
#include "LIMITS.h"

void Callback_Launch_Themis_Client(google::protobuf::Message *message,
	void *user_data) {
	
	KCONTEXT *ctx = (KCONTEXT*) user_data;
	RANA_EXT *rana_ext = new RANA_EXT;
	
	if (message == NULL) {
		std::cout << "request failed" << std::endl;
	} else {
		kible::themis::LaunchResponse response;
		response.CopyFrom(*message);

		ASSERT_E_B((Initialize_RANA_EXT(rana_ext, ctx)),
		"failed to initialize rana_ext", ctx);
	}
}

int main() {
	KCONTEXT *ctx = new KCONTEXT;
	

	LOG_INFO_CTX(ctx) {
		ADD_STR_LOG("message", "main begun");
		ADD_STR_LOG("version", SOFTWARE_VERSION);
	}

	Initialize_KCONTEXT(ctx);

	ctx->core_services_backbone = ROOT_SOCKET_TYPE_WS;

	#ifdef TESTING_BUILD

	ctx->core_services_backbone_port = THEMIS_PORT;
	ctx->http_services_backbone_port = THEMIS_PORT_HTTP;

	ctx->themis_url = "localhost";

	#else

	ctx->core_services_backbone_port = 443;
	ctx->http_services_backbone_port = 443;
	if (!Themis_EDGE_CLIENT(std::string("https://") + rana_ext->ctx->url,
		rana_ext->ctx->uuid, &ctx.themis_url)) {
		
		LOG_ERROR_CTX(rana_ext->ctx) {
			ADD_STR_LOG("message", "Signin failed.");
			ADD_STR_LOG("error", "Couldn't query for edge server");
		}

		cout << "failed to query edge" << endl;
		return false;
	}

	std::string delimiter = "alienhub.xyz";
	ctx->themis_url = ctx.themis_url.substr(0, ctx.themis_url.find(delimiter)) +
		rana_ext->ctx->url + ctx.themis_url.substr(
		ctx.themis_url.find(delimiter) + delimiter.length());

	#endif

	Launch_THEMIS_CLIENT(ctx, Callback_Launch_Themis_Client, (void*) ctx);

	return 0;
}
