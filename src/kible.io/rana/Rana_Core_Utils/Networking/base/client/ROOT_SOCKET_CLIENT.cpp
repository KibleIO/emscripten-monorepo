#include "ROOT_SOCKET_CLIENT.h"

bool Initialize_ROOT_SOCKET_CLIENT(ROOT_SOCKET_CLIENT *client, KCONTEXT *ctx,
	int port, char *address) {

	client->type = ctx->core_services_backbone;

	switch (client->type) {
		case ROOT_SOCKET_TYPE_WS:
			if (!Initialize_WS_CLIENT_MASTER(
				&client->ws_client_master, ctx, port, address)) {
				
				return false;
			}
			client->initialized = true;
			return true;
		default:
			return false;
	}
}

void Delete_ROOT_SOCKET_CLIENT(ROOT_SOCKET_CLIENT *client) {
	switch (client->type) {
		case ROOT_SOCKET_TYPE_WS:
			Delete_WS_CLIENT_MASTER(&client->ws_client_master);
			break;
	}
}