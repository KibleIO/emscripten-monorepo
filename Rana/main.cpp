#include "Rana_Core_Utils/Utilities/ASSERT.h"
#include "Rana_Core_Utils/Utilities/FPS_LIMITER.h"
#include "RANA_EXT.h"
#include "LIMITS.h"

#define LOOP_SPEED_FPS 60

int main() {
	KCONTEXT ctx;
	FPS_LIMITER fps_limiter;
	RANA_EXT rana_ext;
	thread *rana_connect_loop;

	LOG_INFO_CTX(&ctx) {
		ADD_STR_LOG("message", "main begun");
		ADD_STR_LOG("version", SOFTWARE_VERSION);
	}

	ASSERT_E_R((Initialize_RANA_EXT(&rana_ext, &ctx)),
			   "failed to initialize rana_ext", &ctx);
	ASSERT_E_R((Initialize_FPS_LIMITER(&fps_limiter, LOOP_SPEED_FPS, false)),
			   "Failed to intialize fps limiter", &ctx);

	if (!Connect_To_Themis_RANA_EXT(&rana_ext)) {
		Disconnect_From_Themis_RANA_EXT(&rana_ext, false);

		LOG_INFO_CTX(&ctx) {
			ADD_STR_LOG("message", "failed to connect to Themis");
		}

		return 0;
	}

	while (Running_RANA_EXT(&rana_ext)) {
		Start_FPS_LIMITER(&fps_limiter);

		Render_RANA_EXT(&rana_ext);

		Themis_Status_RANA_EXT(&rana_ext);

		Stop_FPS_LIMITER(&fps_limiter);
	}

	Disconnect_From_Themis_RANA_EXT(&rana_ext, false);

	Delete_FPS_LIMITER(&fps_limiter);
	Delete_RANA_EXT(&rana_ext);

	LOG_INFO_CTX(&ctx) { ADD_STR_LOG("message", "clean exit"); }

	return 0;
}
