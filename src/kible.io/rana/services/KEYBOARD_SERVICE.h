#ifndef KEYBOARD_SERVICE_H_
#define KEYBOARD_SERVICE_H_

#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"

struct KEYBOARD_SERVICE {
	KCONTEXT *ctx;
	CLIENT *c;
};

bool Initialize_KEYBOARD_SERVICE(KEYBOARD_SERVICE*, KCONTEXT*);
bool Connect_KEYBOARD_SERVICE(KEYBOARD_SERVICE*, CLIENT*);
void Disconnect_KEYBOARD_SERVICE(KEYBOARD_SERVICE*);
void Delete_KEYBOARD_SERVICE(KEYBOARD_SERVICE*);

#endif
