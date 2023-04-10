#include "Rana_Core_Utils/Utilities/ASSERT.h"
#include "Rana_Core_Utils/Utilities/FPS_LIMITER.h"
#include "RANA_EXT.h"
#include "LIMITS.h"

int main() {
	KCONTEXT ctx;
	RANA_EXT rana_ext;

	LOG_INFO_CTX(&ctx) {
		ADD_STR_LOG("message", "main begun");
		ADD_STR_LOG("version", SOFTWARE_VERSION);
	}
	
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
