#include "ROOT_SOCKET_CLIENT.h"

bool Initialize_ROOT_SOCKET_CLIENT(ROOT_SOCKET_CLIENT *CLIENT, KCONTEXT *ctx,
	int port, char *address) {

	CLIENT->type = ctx->core_services_backbone;

	switch (CLIENT->type) {
		case ROOT_SOCKET_TYPE_WS:
			if (!Initialize_WS_CLIENT_MASTER(
				&CLIENT->ws_client_master, ctx, port, address)) {
				
				return false;
			}
			CLIENT->initialized = true;
			return true;
		default:
			return false;
	}
}

void Delete_ROOT_SOCKET_CLIENT(ROOT_SOCKET_CLIENT *CLIENT) {
	switch (CLIENT->type) {
		case ROOT_SOCKET_TYPE_WS:
			Delete_WS_CLIENT_MASTER(&CLIENT->ws_client_master);
			break;
	}
}