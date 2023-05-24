#include "SERVICE_CLIENT.h"

bool Initialize_SERVICE_CLIENT_REGISTRY(SERVICE_CLIENT_REGISTRY *registry,
	KCONTEXT *ctx, int count, ...) {

	registry->service_count = 0;

	for (int i = 0; i < MAX_SERVICES; i++) {
		registry->service_client[i] = NULL;
	}

	registry->socket_client_registry = new SOCKET_CLIENT_REGISTRY;	
	if (!Initialize_SOCKET_CLIENT_REGISTRY(
		registry->socket_client_registry, ctx)) {
		
		return false;
	}

	va_list args;
	va_start(args, count);
	for (int i = 0; i < count; i++) {
		registry->service_client[i] = va_arg(args, SERVICE_CLIENT*);
		if (!registry->service_client[i]->Initialize(ctx, registry)) {
			return false;
		}
		registry->service_count++;
	}
	va_end(args);

	return true;
}

void Delete_SERVICE_CLIENT_REGISTRY(SERVICE_CLIENT_REGISTRY *registry) {
	for (int i = 0; i < MAX_SERVICES; i++) {
		if (registry->service_client[i] != NULL) {
			registry->service_client[i]->Delete();
			delete registry->service_client[i];
			registry->service_client[i] = NULL;
		}
	}

	//Delete_SOCKET_CLIENT_REGISTRY(&registry->socket_client_registry);
}