#ifndef KCONTEXT_H_
#define KCONTEXT_H_

#include <string>
#include "SCREEN_DIM.h"
#include "UTILS.h"
#include "LOGGING.h"

#define LOGGER_INFO(ctx, ...) Log_LOGGING(&ctx->logging, __VA_ARGS__, "INFO", __FILE__, __LINE__, __func__)
#define LOGGER_WARN(ctx, ...) Log_LOGGING(&ctx->logging, __VA_ARGS__, "WARN", __FILE__, __LINE__, __func__)
#define LOGGER_ERROR(ctx, ...) Log_LOGGING(&ctx->logging, __VA_ARGS__, "ERROR", __FILE__, __LINE__, __func__)

struct KCONTEXT{
	char trace_uuid[UUID_STR_SIZE];
	char core_system[CORE_SYSTEM_STR_SIZE];
	SCREEN_DIM screen_dim;
	bool screen_dim_changed;
	std::string uuid;
	std::string url;
	std::string themis_url;
	int core_services_backbone;
	int core_services_backbone_port;
	int http_services_backbone_port;
	LOGGING logging;
};

void Initialize_KCONTEXT(KCONTEXT*, char*);
SCREEN_DIM Get_Screen_Dim_KCONTEXT(KCONTEXT*);
void Set_Screen_Dim_KCONTEXT(KCONTEXT*, SCREEN_DIM);

#endif