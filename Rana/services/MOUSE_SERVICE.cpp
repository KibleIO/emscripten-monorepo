#include "MOUSE_SERVICE.h"

bool Initialize_MOUSE_SERVICE(MOUSE_SERVICE *mouse, KCONTEXT *ctx) {
	mouse->ctx = ctx;

	return true;
}

bool Connect_MOUSE_SERVICE(MOUSE_SERVICE *mouse, CLIENT *c) {
	ASSERT_E_R(c != NULL, "Client is NULL", mouse->ctx);

	mouse->c = c;

	return true;
}

void Disconnect_MOUSE_SERVICE(MOUSE_SERVICE *mouse) {
	(void) mouse; //nop
}

void Delete_MOUSE_SERVICE(MOUSE_SERVICE *mouse) {

}
