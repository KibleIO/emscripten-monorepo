#include "MOUSE_CLIENT.h"

void Recv_Callback_MOUSE_CLIENT(void *user_ptr, char *buffer, int buffer_size) {
	//nop
}

bool MOUSE_CLIENT::Initialize(KCONTEXT *ctx, SERVICE_CLIENT_REGISTRY *registry) {
	/*
	if (!Initialize_SOCKET_CLIENT(&socket_client,
		Recv_Callback_MOUSE_CLIENT, &registry->socket_client_registry,
		ctx, this)) {
		
		return false;
	}
	*/

	if (!Initialize_SOCKET_CLIENT(&socket_client,
		Recv_Callback_MOUSE_CLIENT, registry->ws_client_master,
		ctx, this)) {
		
		return false;
	}

	std::cout << "MOUSE SETUP " << (void*) &socket_client << std::endl;

	return true;
}

void MOUSE_CLIENT::Delete() {
}

void Send_Event_MOUSE_CLIENT(MOUSE_CLIENT *client, MOUSE_EVENT_T *event) {
	std::cout << "sending mouse " << (void*) &client->socket_client << std::endl;
	Send_SOCKET_CLIENT(&client->socket_client, (char*) event,
		sizeof(MOUSE_EVENT_T));
}