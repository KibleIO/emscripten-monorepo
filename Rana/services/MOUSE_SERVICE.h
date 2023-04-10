#ifndef MOUSE_SERVICE_H_
#define MOUSE_SERVICE_H_

#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"

struct MOUSE_SERVICE {
	KCONTEXT *ctx;
	CLIENT *c;
};

bool Initialize_MOUSE_SERVICE(MOUSE_SERVICE*, KCONTEXT*);
bool Connect_MOUSE_SERVICE(MOUSE_SERVICE*, CLIENT*);
void Disconnect_MOUSE_SERVICE(MOUSE_SERVICE*);
void Delete_MOUSE_SERVICE(MOUSE_SERVICE*);

#endif
