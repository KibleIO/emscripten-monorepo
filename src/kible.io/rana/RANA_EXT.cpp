#include "RANA_EXT.h"

bool Initialize_RANA_EXT(RANA_EXT *rana, KCONTEXT *ctx) {
	if (!Initialize_SERVICE_CLIENT_REGISTRY(&rana->registry, ctx, 4,
		new MOUSE_CLIENT,
		new KEYBOARD_CLIENT,
		new VIDEO_CLIENT,
		new AUDIO_CLIENT)) {
		
		return false;
	}

	return true;
}