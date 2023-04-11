#ifndef AUDIO_SERVICE_H_
#define AUDIO_SERVICE_H_

#include <thread>
#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"

#define MAX_NAL_SIZE 50000

struct AUDIO_SERVICE {
	KCONTEXT *ctx;
	CLIENT *c;
	thread *main_loop;
	volatile bool main_loop_running;
	char nal_buffer[MAX_NAL_SIZE];
};

bool Initialize_AUDIO_SERVICE(AUDIO_SERVICE*, KCONTEXT*);
bool Connect_AUDIO_SERVICE(AUDIO_SERVICE*, CLIENT*);
void Disconnect_AUDIO_SERVICE(AUDIO_SERVICE*);
void Delete_AUDIO_SERVICE(AUDIO_SERVICE*);

#endif
