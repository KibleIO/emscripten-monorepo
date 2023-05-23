#include "SOCKET_CLIENT.h"

bool Initialize_SOCKET_CLIENT(SOCKET_CLIENT *CLIENT,
	Receive_Callback_SOCKET_CLIENT callback,
	SOCKET_CLIENT_REGISTRY *registry, KCONTEXT *ctx, void *user_ptr) {

	CLIENT->type = ctx->core_services_backbone;

	switch (CLIENT->type) {
		case ROOT_SOCKET_TYPE_WS:
			return Initialize_WS_CLIENT(
				&CLIENT->ws_client, callback, registry, ctx,
				user_ptr);
		default:
			return false;
	}

}

bool Send_SOCKET_CLIENT(SOCKET_CLIENT *CLIENT, char *buffer, int size) {
	switch (CLIENT->type) {
		case ROOT_SOCKET_TYPE_WS:
			return Send_WS_CLIENT(&CLIENT->ws_client, buffer, size);
		default:
			return false;
	}
}

void Delete_SOCKET_CLIENT(SOCKET_CLIENT *CLIENT) {
	switch (CLIENT->type) {
		case ROOT_SOCKET_TYPE_WS:
			Delete_WS_CLIENT(&CLIENT->ws_client);
	}
}