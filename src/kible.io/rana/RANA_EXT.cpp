#include "RANA_EXT.h"

bool Initialize_RANA_EXT(RANA_EXT *rana, KCONTEXT *ctx) {
	if (!Initialize_SERVICE_CLIENT_REGISTRY(&rana->registry, ctx, 5,
		new MOUSE_CLIENT,
		new KEYBOARD_CLIENT,
		new VIDEO_CLIENT,
		new AUDIO_CLIENT,
		new KEEP_ALIVE_CLIENT)) {
		

		Show_Error_Message(std::string("Failed to initialize Rana"));
		return false;
	}

	Hide_Loading_Wheel();

	return true;
}