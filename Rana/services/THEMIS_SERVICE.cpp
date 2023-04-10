#include "THEMIS_SERVICE.h"

bool Initialize_THEMIS_SERVICE(THEMIS_SERVICE *themis, KCONTEXT *ctx) {
	themis->ctx = ctx;
	themis->themis_status = HERMES_STATUS_DISCONNECTED;
	themis->main_loop = NULL;
	themis->main_loop_running = false;

	ASSERT_E_R((Initialize_FPS_LIMITER(&themis->fps_limiter,
		HERMES_STATUS_FPS, false)), "Failed to intialize fps limiter",
		themis->ctx);

	return true;
}

void Main_Loop_THEMIS_SERVICE(THEMIS_SERVICE *themis) {
	while (themis->main_loop_running) {
		Start_FPS_LIMITER(&themis->fps_limiter);

		themis->themis_status = Status_HERMES_CLIENT(themis->client);

		if (themis->themis_status != HERMES_STATUS_NORMAL) {
			themis->main_loop_running = false;
			break;
		}

		Stop_FPS_LIMITER(&themis->fps_limiter);
	}
}

bool Connect_THEMIS_SERVICE(THEMIS_SERVICE *themis, CLIENT *c,
	HERMES_CLIENT *client) {

	SCREEN_DIM screen_dim = SCREEN_DIM{
		.bw = 800, .sw = 800, .h = 600
	};

	/*screen_dim.sw *= SCALE_RATIO;
	screen_dim.bw *= SCALE_RATIO;
	screen_dim.h *= SCALE_RATIO;*/

	themis->themis_status = HERMES_STATUS_NORMAL;
	themis->client = client;

	ASSERT_E_R(c != NULL, "Client is NULL", themis->ctx);
	ASSERT_E_R(Send_CLIENT(c, (char*)&screen_dim, sizeof(SCREEN_DIM)),
		"Couldn't send screen_dim", themis->ctx);

	themis->main_loop_running = true;
	themis->main_loop = new thread(Main_Loop_THEMIS_SERVICE, themis);

	return true;
}

uint8_t Themis_Status_THEMIS_SERVICE(THEMIS_SERVICE *themis) {
	return themis->themis_status;
}

void Disconnect_THEMIS_SERVICE(THEMIS_SERVICE *themis) {
	themis->themis_status = HERMES_STATUS_DISCONNECTED;

	themis->main_loop_running = false;
	if (themis->main_loop != NULL) {
		themis->main_loop->join();
		delete themis->main_loop;
		themis->main_loop = NULL;
	}
}

void Delete_THEMIS_SERVICE(THEMIS_SERVICE *themis) {
	Delete_FPS_LIMITER(&themis->fps_limiter);
}
