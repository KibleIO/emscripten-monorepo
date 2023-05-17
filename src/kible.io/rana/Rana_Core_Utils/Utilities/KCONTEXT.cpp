#include "KCONTEXT.h"

void Initialize_KCONTEXT(KCONTEXT *ctx) {
	char cookie[MAX_COOKIE_SIZE];

	ctx->screen_dim_changed = false;

	Get_Cookie(cookie);
	ctx->uuid = cookie;

	Get_Url(cookie);
	ctx->url = cookie;
}

SCREEN_DIM Get_Screen_Dim_KCONTEXT(KCONTEXT *ctx) { return ctx->screen_dim; }

void Set_Screen_Dim_KCONTEXT(KCONTEXT *ctx, SCREEN_DIM screen_dim) {
	ctx->screen_dim_changed = true;
	ctx->screen_dim = screen_dim;
}