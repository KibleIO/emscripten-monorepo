#ifndef THEMIS_SERVICE_H_
#define THEMIS_SERVICE_H_

#include <thread>
#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/FPS_LIMITER.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Utilities/SCREEN_DIM.h"
#include "../Rana_Core_Utils/Hermes/HERMES_CLIENT.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"
#include "../LIMITS.h"

#define HERMES_STATUS_FPS 60

struct THEMIS_SERVICE {
	KCONTEXT *ctx;
	HERMES_CLIENT *client;
	uint8_t themis_status;
	FPS_LIMITER fps_limiter;
	thread *main_loop;
	volatile bool main_loop_running;
};

bool Initialize_THEMIS_SERVICE(THEMIS_SERVICE*, KCONTEXT*);
void Main_Loop_THEMIS_SERVICE(THEMIS_SERVICE*);
bool Connect_THEMIS_SERVICE(THEMIS_SERVICE*, CLIENT*, HERMES_CLIENT*);
uint8_t Themis_Status_THEMIS_SERVICE(THEMIS_SERVICE*);
void Disconnect_THEMIS_SERVICE(THEMIS_SERVICE*);
void Delete_THEMIS_SERVICE(THEMIS_SERVICE*);

#endif
