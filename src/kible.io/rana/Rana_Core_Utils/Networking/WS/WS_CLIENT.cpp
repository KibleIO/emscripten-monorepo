#include "WS_CLIENT.h"

bool Initialize_WS_CLIENT(WS_CLIENT *client,
	Receive_Callback_SOCKET_CLIENT callback,
	SOCKET_CLIENT_REGISTRY *registry, KCONTEXT *ctx, void *user_ptr) {
	
	client->ctx = ctx;
	Set_Name_WS_CLIENT(client, "unknown");

	client->ws_master = 
		&registry->root_sockets[ROOT_SOCKET_TYPE_WS].ws_client_master;

	if (!Set_Recv_Timeout_WS_CLIENT(client, DEFAULT_RECV_TIMEOUT, 0)) {
		return false;
	}

	client->client_id = Register_Vhost_WS_CLIENT_MASTER(client->ws_master,
		callback, user_ptr);

	return true;
}

void Set_Name_WS_CLIENT(WS_CLIENT *client, char *name) {
	strcpy(client->name, name);
}

bool Set_Recv_Timeout_WS_CLIENT(WS_CLIENT *client, int sec, int usec) {
	client->recv_timeout = (sec * 1000) + (usec / 1000);
	return true;
}

bool Set_High_Priority_WS_CLIENT(WS_CLIENT *client) {
	return true;
}

bool Send_WS_CLIENT(WS_CLIENT *client, char *buffer, int size) {
	return Send_WS_CLIENT_MASTER(client->ws_master, (uint8_t*) buffer, size,
		client->client_id);
}

void Delete_WS_CLIENT(WS_CLIENT *client) {
}