#include "KEYBOARD_CLIENT.h"

void Recv_Callback_KEYBOARD_CLIENT(void *user_ptr, char *buffer, int buffer_size) {
	//nop
}

bool KEYBOARD_CLIENT::Initialize(KCONTEXT *ctx, SERVICE_CLIENT_REGISTRY *registry) {
	if (!Initialize_SOCKET_CLIENT(&socket_client,
		Recv_Callback_KEYBOARD_CLIENT, &registry->socket_client_registry,
		ctx, this)) {
		
		return false;
	}

	return true;
}

void KEYBOARD_CLIENT::Delete() {
}
