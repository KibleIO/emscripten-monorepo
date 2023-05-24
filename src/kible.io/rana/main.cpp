#include "RANA_EXT.h"
#include "Rana_Core_Utils/Utilities/ASSERT.h"
#include "Rana_Core_Utils/Utilities/UTILS.h"

int main() {
	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());

	KCONTEXT ctx;
	RANA_EXT rana_ext;

	LOG_INFO_CTX(&ctx) {
		ADD_STR_LOG("message", "main begun");
		ADD_STR_LOG("version", SOFTWARE_VERSION);
	}

	Initialize_KCONTEXT(&ctx);

	ctx.core_services_backbone = ROOT_SOCKET_TYPE_WS;

	#ifdef TESTING_BUILD

	ctx.core_services_backbone_port = THEMIS_PORT;
	ctx.http_services_backbone_port = THEMIS_PORT_HTTP;

	ctx.themis_url = "localhost";

	#else

	ctx.core_services_backbone_port = 443;
	ctx.http_services_backbone_port = 443;
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
	ctx.themis_url = ctx.themis_url.substr(0, ctx.themis_url.find(delimiter)) +
		rana_ext->ctx->url + ctx.themis_url.substr(
		ctx.themis_url.find(delimiter) + delimiter.length());

	#endif

	ASSERT_E_R((Launch_THEMIS_CLIENT(&ctx)),
		"failed to launch themis", &ctx);

	ASSERT_E_R((Initialize_RANA_EXT(&rana_ext, &ctx)),
		"failed to initialize rana_ext", &ctx);
	
	/*
	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());

	SERVICE_CLIENT_REGISTRY registry;
	MOUSE_CLIENT mouse;
	KEYBOARD_CLIENT keyboard;
	VIDEO_CLIENT video;
	AUDIO_CLIENT audio;

	if (!Initialize_SERVICE_CLIENT_REGISTRY(&registry, &ctx, 4,
		&mouse,
		&keyboard,
		&video,
		&audio)) {
		
		return false;
	}

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());
	*/

	return 0;
}
