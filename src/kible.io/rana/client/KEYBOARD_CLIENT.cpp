#include "KEYBOARD_CLIENT.h"

void Recv_Callback_KEYBOARD_CLIENT(void *user_ptr, char *buffer, int buffer_size) {
	//nopp
}

bool KEYBOARD_CLIENT::Initialize(KCONTEXT *ctx, SERVICE_CLIENT_REGISTRY *registry) {
	if (!Initialize_SOCKET_CLIENT(&socket_client,
		Recv_Callback_KEYBOARD_CLIENT, registry->socket_client_registry,
		ctx, this)) {
		
		return false;
	}

	return true;
}

void KEYBOARD_CLIENT::Delete() {
}

void Send_Event_KEYBOARD_CLIENT(KEYBOARD_CLIENT *client, KEYBOARD_EVENT_T *event) {
	Send_SOCKET_CLIENT(&client->socket_client, (char*) event,
		sizeof(KEYBOARD_EVENT_T));
}