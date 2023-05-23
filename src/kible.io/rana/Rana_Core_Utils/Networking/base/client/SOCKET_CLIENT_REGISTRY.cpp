#include "SOCKET_CLIENT_REGISTRY.h"

bool Initialize_SOCKET_CLIENT_REGISTRY(SOCKET_CLIENT_REGISTRY *registry) {
	for (int i = 0; i < MAX_ROOT_SOCKETS; i++) {
		registry->root_sockets[i].initialized = false;
	}
	return true;
}

void Delete_SOCKET_CLIENT_REGISTRY(SOCKET_CLIENT_REGISTRY *registry) {
	for (int i = 0; i < MAX_ROOT_SOCKETS; i++) {
		if (registry->root_sockets[i].initialized) {
			Delete_ROOT_SOCKET_CLIENT(&registry->root_sockets[i]);
		}
	}
}