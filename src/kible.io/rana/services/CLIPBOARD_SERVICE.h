#ifndef CLIPBOARD_SERVICE_H_
#define CLIPBOARD_SERVICE_H_

#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"

#define MAX_CLIPBOARD_SIZE 256

struct CLIPBOARD_SERVICE {
	KCONTEXT *ctx;
	CLIENT *c;
	thread *main_loop;
	volatile bool main_loop_running;
};

bool Initialize_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE*, KCONTEXT*);
void Main_Loop_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE*);
bool Connect_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE*, CLIENT*);
void Disconnect_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE*);
void Delete_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE*);

#endif
