#include "KEEP_ALIVE_CLIENT.h"

void generic_stuff_iguess(emscripten_fetch_t *fetch) {
	KEEP_ALIVE_CLIENT *keep = (KEEP_ALIVE_CLIENT*) fetch->userData;

	if (fetch->data == NULL) {
		std::cout << "call failed" << std::endl;
	} else {
		std::cout << "call was a success " << fetch->status << " " << fetch->data << std::endl;
	}

	keep->testing = !keep->testing;

	emscripten_fetch_close(fetch);
}

void Callback_KEEP_ALIVE_CLIENT(void *user_data) {
	std::cout << "heere" << std::endl;
	KEEP_ALIVE_CLIENT *client = (KEEP_ALIVE_CLIENT*) user_data;
	
	HTTP_Request_Temp(generic_stuff_iguess, client);
	
	emscripten_async_call(Callback_KEEP_ALIVE_CLIENT, user_data,
		KEEP_ALIVE_CLIENT_CALLBACK_INTERVAL_MS);

	std::cout << "did a ping" << std::endl;
	std::cout << "did a ping2 " << (void*) client->ctx << std::endl;
}

bool KEEP_ALIVE_CLIENT::Initialize(KCONTEXT *ctx_in, SERVICE_CLIENT_REGISTRY *registry) {
	ctx = ctx_in;
	emscripten_async_call(Callback_KEEP_ALIVE_CLIENT, (void*) this,
		KEEP_ALIVE_CLIENT_CALLBACK_INTERVAL_MS);
	return true;
}

void KEEP_ALIVE_CLIENT::Delete() {
}