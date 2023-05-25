#include "KEEP_ALIVE_CLIENT.h"

void Callback_KEEP_ALIVE_CLIENT(void *user_data) {
	KEEP_ALIVE_CLIENT *client = (KEEP_ALIVE_CLIENT*) user_data;
	int width;
	int height;

	Ping_THEMIS_CLIENT(client->ctx);

	SCREEN_DIM dim = Get_Screen_Dim_KCONTEXT(client->ctx);
	get_screen_width_height(&width, &height);

	if (width != dim.bw || height != dim.h) {
		Set_Screen_Dim_KCONTEXT(client->ctx, (SCREEN_DIM) {
		width,
		width,
		height});

		Dimensions_THEMIS_CLIENT(client->ctx, width, height);
	}
	
	emscripten_async_call(Callback_KEEP_ALIVE_CLIENT, user_data,
		KEEP_ALIVE_CLIENT_CALLBACK_INTERVAL_MS);
}

bool KEEP_ALIVE_CLIENT::Initialize(KCONTEXT *ctx_in, SERVICE_CLIENT_REGISTRY *registry) {
	ctx = ctx_in;
	emscripten_async_call(Callback_KEEP_ALIVE_CLIENT, (void*) this,
		0); //0 the first time for a quick keep alive signal
	return true;
}

void KEEP_ALIVE_CLIENT::Delete() {
}