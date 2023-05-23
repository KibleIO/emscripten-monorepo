#include "RANA_EXT.h"

bool Initialize_RANA_EXT(RANA_EXT *rana, KCONTEXT *ctx) {
	if (!Initialize_SERVICE_CLIENT_REGISTRY(&rana->registry, ctx, 5,
		new THEMIS_CLIENT,
		new VIDEO_CLIENT,
		new AUDIO_CLIENT,
		new MOUSE_CLIENT,
		new KEYBOARD_CLIENT)) {
		
		return false;
	}

	return true;
}