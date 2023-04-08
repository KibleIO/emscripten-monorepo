#ifndef AUDIO_SERVICE_H_
#define AUDIO_SERVICE_H_

#include <thread>
#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"

struct AUDIO_SERVICE {
	KCONTEXT *ctx;
	CLIENT *c;
};

bool Initialize_AUDIO_SERVICE(AUDIO_SERVICE*, KCONTEXT*);
bool Connect_AUDIO_SERVICE(AUDIO_SERVICE*, CLIENT*);
void Disconnect_AUDIO_SERVICE(AUDIO_SERVICE*);
void Delete_AUDIO_SERVICE(AUDIO_SERVICE*);

#endif
