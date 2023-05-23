#include "SERVICE_CLIENT.h"

bool Initialize_SERVICE_CLIENT_REGISTRY(SERVICE_CLIENT_REGISTRY *registry,
	KCONTEXT *ctx, int count, ...) {

	registry->service_count = 0;

	for (int i = 0; i < MAX_SERVICES; i++) {
		registry->service_client[i] = NULL;
	}

	if (!Initialize_SOCKET_CLIENT_REGISTRY(
		&registry->socket_client_registry)) {
		
		return false;
	}

	if (!Initialize_ROOT_SOCKET_CLIENT(
		&registry->socket_client_registry.root_sockets
		[ROOT_SOCKET_TYPE_WS], ctx, ctx->core_services_backbone_port,
		(char*) ctx->themis_url.c_str())) {
		
		return false;
	}

	va_list args;
	va_start(args, count);
	for (int i = 0; i < count; i++) {
		registry->service_client[i] = va_arg(args, SERVICE_CLIENT*);
		if (!registry->service_client[i]->Initialize(ctx, registry)) {
			return false;
		}
	}
	va_end(args);

	registry->service_count = count;

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

	Delete_SOCKET_CLIENT_REGISTRY(&registry->socket_client_registry);
}