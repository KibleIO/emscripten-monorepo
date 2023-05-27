#include "KCONTEXT.h"

void Initialize_KCONTEXT(KCONTEXT *ctx, char *core_system) {
	char cookie[MAX_COOKIE_SIZE];

	ctx->screen_dim_changed = false;

	Get_Cookie(cookie);
	ctx->uuid = cookie;

	Get_Url(cookie);
	ctx->url = cookie;

	generate_uuid(ctx->trace_uuid);
	strcpy(ctx->core_system, core_system);

	if (!Initialize_LOGGING(&ctx->logging, std::string(ctx->core_system),
		std::string(ctx->trace_uuid), ctx->uuid)) {
		
		std::cout << "failed to initialize logger" << std::endl;
	}
}

SCREEN_DIM Get_Screen_Dim_KCONTEXT(KCONTEXT *ctx) { return ctx->screen_dim; }

void Set_Screen_Dim_KCONTEXT(KCONTEXT *ctx, SCREEN_DIM screen_dim) {
	ctx->screen_dim_changed = true;
	ctx->screen_dim = screen_dim;
}