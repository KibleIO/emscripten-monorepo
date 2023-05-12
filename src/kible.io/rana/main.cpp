#include "RANA_EXT.h"
#include "Rana_Core_Utils/Utilities/ASSERT.h"

int main() {
	char cookie_value[MAX_COOKIE_SIZE];
	get_cookie(cookie_value);
	std::cout << "cookie: " << cookie_value << std::endl;

	KCONTEXT ctx;
	RANA_EXT rana_ext;

	LOG_INFO_CTX(&ctx) {
		ADD_STR_LOG("message", "main begun");
		ADD_STR_LOG("version", SOFTWARE_VERSION);
	}

	Initialize_KCONTEXT(&ctx);

	ASSERT_E_R((Initialize_RANA_EXT(&rana_ext, &ctx)),
			   "failed to initialize rana_ext", &ctx);

	if (!Connect_To_Themis_RANA_EXT(&rana_ext)) {
		Disconnect_From_Themis_RANA_EXT(&rana_ext, false);

		LOG_INFO_CTX(&ctx) {
			ADD_STR_LOG("message", "failed to connect to Themis");
		}

		return 0;
	}

	LOG_INFO_CTX(&ctx) { ADD_STR_LOG("message", "clean exit"); }

	return 0;
}
