#include "SOCKET_CLIENT.h"

bool Initialize_SOCKET_CLIENT(SOCKET_CLIENT *client,
	Receive_Callback_SOCKET_CLIENT callback,
	SOCKET_CLIENT_REGISTRY *registry, KCONTEXT *ctx, void *user_ptr) {

	client->type = ctx->core_services_backbone;

	switch (client->type) {
		case ROOT_SOCKET_TYPE_WS:
			return Initialize_WS_CLIENT(
				&client->ws_client, callback, registry, ctx,
				user_ptr);
		default:
			return false;
	}

}

bool Send_SOCKET_CLIENT(SOCKET_CLIENT *client, char *buffer, int size) {
	switch (client->type) {
		case ROOT_SOCKET_TYPE_WS:
			return Send_WS_CLIENT(&client->ws_client, buffer, size);
		default:
			return false;
	}
}

void Delete_SOCKET_CLIENT(SOCKET_CLIENT *client) {
	switch (client->type) {
		case ROOT_SOCKET_TYPE_WS:
			Delete_WS_CLIENT(&client->ws_client);
	}
}