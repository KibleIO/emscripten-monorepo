#include "SOCKET_CLIENT_REGISTRY.h"

bool Initialize_SOCKET_CLIENT_REGISTRY(SOCKET_CLIENT_REGISTRY *registry,
	KCONTEXT *ctx) {
	
	for (int i = 0; i < MAX_ROOT_SOCKETS; i++) {
		registry->root_sockets[i] = NULL;
	}

	switch (ctx->core_services_backbone) {
		case ROOT_SOCKET_TYPE_WS:
			registry->root_sockets[ROOT_SOCKET_TYPE_WS] =
				new ROOT_SOCKET_CLIENT;
			if (!Initialize_ROOT_SOCKET_CLIENT(
				registry->root_sockets[ROOT_SOCKET_TYPE_WS],
				ctx, ctx->core_services_backbone_port,
				(char*) ctx->themis_url.c_str())) {
				
				return false;
			}
			return true;
		default:
			return false;
	}

	return true;
}

void Delete_SOCKET_CLIENT_REGISTRY(SOCKET_CLIENT_REGISTRY *registry) {
	for (int i = 0; i < MAX_ROOT_SOCKETS; i++) {
		if (registry->root_sockets[i] != NULL) {
			Delete_ROOT_SOCKET_CLIENT(registry->root_sockets[i]);
		}
	}
}