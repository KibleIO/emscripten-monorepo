#include "KCONTEXT.h"

void Initialize_KCONTEXT(KCONTEXT *ctx, char *core_system, bool production) {
	char cookie[MAX_COOKIE_SIZE];
	ctx->production = production;
	ctx->screen_dim_changed = false;

	Get_Cookie(cookie);
	ctx->uuid = cookie;

	Get_Url(cookie);
	ctx->url = cookie;

	generate_uuid(ctx->trace_uuid);
	strcpy(ctx->core_system, core_system);
}

SCREEN_DIM Get_Screen_Dim_KCONTEXT(KCONTEXT *ctx) { return ctx->screen_dim; }

void Set_Screen_Dim_KCONTEXT(KCONTEXT *ctx, SCREEN_DIM screen_dim) {
	ctx->screen_dim_changed = true;
	ctx->screen_dim = screen_dim;
}

void Log_KCONTEXT(KCONTEXT *ctx, nlohmann::json obj, char *level, char *file,
	int line, const char *function) {

	char time_str[CURRENT_TIME_LEN];

	get_current_time(time_str);

	obj["@_timestamp"] = std::string(time_str);
	obj["file"] = std::string(file);
	obj["line"] = line;
	obj["function"] = std::string(function);
	obj["system"] = std::string(ctx->core_system);
	obj["trace_uuid"] = std::string(ctx->trace_uuid);
	obj["uuid"] = ctx->uuid;
	obj["type"] = std::string(level);

	std::string elk_url = ELASTIC_SEARCH_URL;
	std::string delimiter = "hub.alienhub.xyz";
	elk_url = elk_url.substr(0,
		elk_url.find(delimiter)) + ctx->url +
		elk_url.substr(elk_url.find(delimiter) +
		delimiter.length());

	if (ctx->production) {
		Post_ELASTIC_SEARCH_CLIENT(elk_url, (char*) obj.dump().c_str());
	} else {
		std::cout << obj.dump() << std::endl;
	}
}