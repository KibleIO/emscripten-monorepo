#include "KEYBOARD_SERVICE.h"

bool Initialize_KEYBOARD_SERVICE(KEYBOARD_SERVICE *keyboard, KCONTEXT *ctx) {
	keyboard->ctx = ctx;

	return true;
}

bool Connect_KEYBOARD_SERVICE(KEYBOARD_SERVICE *keyboard, CLIENT *c) {
	ASSERT_E_R(c != NULL, "Client is NULL", keyboard->ctx);

	keyboard->c = c;

	return true;
}

void Disconnect_KEYBOARD_SERVICE(KEYBOARD_SERVICE *keyboard) {
	(void) keyboard; //nop
}

void Delete_KEYBOARD_SERVICE(KEYBOARD_SERVICE *keyboard) {

}
