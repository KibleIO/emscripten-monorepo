#include "SERVICE_CLIENT.h"

bool Initialize_SERVICE_CLIENT_REGISTRY(SERVICE_CLIENT_REGISTRY *registry,
	KCONTEXT *ctx, int count, ...) {

	registry->service_count = 0;

	for (int i = 0; i < MAX_SERVICES; i++) {
		registry->service_client[i] = NULL;
	}

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());


	if (!Initialize_SOCKET_CLIENT_REGISTRY(
		&registry->socket_client_registry)) {
		
		return false;
	}

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());


	if (!Initialize_ROOT_SOCKET_CLIENT(
		&registry->socket_client_registry.root_sockets
		[ROOT_SOCKET_TYPE_WS], ctx, ctx->core_services_backbone_port,
		(char*) ctx->themis_url.c_str())) {
		
		return false;
	}

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());


	va_list args;
	va_start(args, count);
	for (int i = 0; i < count; i++) {
		printf("Total memory: %u bytes\n", getTotalMemory());
		printf("Free memory: %u bytes\n", getFreeMemory());
		printf("Used: %d %u bytes (%.2f%%)\n", i, getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());

		registry->service_client[i] = va_arg(args, SERVICE_CLIENT*);
		if (!registry->service_client[i]->Initialize(ctx, registry)) {
			return false;
		}
		printf("Total memory: %u bytes\n", getTotalMemory());
		printf("Free memory: %u bytes\n", getFreeMemory());
		printf("Used: %d %u bytes (%.2f%%)\n", i, getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());

		registry->service_count++;
	}
	va_end(args);

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());

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