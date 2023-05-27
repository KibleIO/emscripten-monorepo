#include "THEMIS_CLIENT.h"

void Launch_THEMIS_CLIENT(KCONTEXT *ctx, HTTP_Protobuf_Callback callback,
	void *user_data) {
	
	pb::THEMIS_CLIENT client_t;
	int width;
	int height;
	std::string url;
	kible::themis::LaunchRequest request;

	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url;

	#endif

	get_screen_width_height(&width, &height);
	Set_Screen_Dim_KCONTEXT(ctx, (SCREEN_DIM) {
		width,
		width,
		height});
	
	request.set_width(width);
	request.set_height(height);
	request.set_launchbackend(
		kible::themis::LaunchBackend::LAUNCHBACKEND_WS);

	pb::Initialize_THEMIS_CLIENT(&client_t, url);
	pb::Launch_THEMIS_CLIENT(&client_t, &request, callback, user_data);
	pb::Delete_THEMIS_CLIENT(&client_t);
}

void Ping_THEMIS_CLIENT(KCONTEXT *ctx) {
	std::string url;

	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url;

	#endif

	pb::THEMIS_CLIENT client;
	kible::themis::PingRequest request;

	pb::Initialize_THEMIS_CLIENT(&client, url);
	pb::Ping_THEMIS_CLIENT(&client, &request);
	pb::Delete_THEMIS_CLIENT(&client);
}

void Density_THEMIS_CLIENT(KCONTEXT *ctx, kible::themis::PixelDensity density) {
	pb::THEMIS_CLIENT client_t;
	std::string url;
	kible::themis::DensityRequest request;

	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url;

	#endif

	request.set_density(density);

	pb::Initialize_THEMIS_CLIENT(&client_t, url);
	pb::Density_THEMIS_CLIENT(&client_t, &request);
	pb::Delete_THEMIS_CLIENT(&client_t);
}

void Dimensions_THEMIS_CLIENT(KCONTEXT *ctx, int width, int height) {
	pb::THEMIS_CLIENT client_t;
	std::string url;
	kible::themis::DimensionsRequest request;

	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url;

	#endif

	request.set_width(width);
	request.set_height(height);

	pb::Initialize_THEMIS_CLIENT(&client_t, url);
	pb::Dimensions_THEMIS_CLIENT(&client_t, &request);
	pb::Delete_THEMIS_CLIENT(&client_t);
}

void FPS_THEMIS_CLIENT(KCONTEXT *ctx, int fps) {
	pb::THEMIS_CLIENT client_t;
	std::string url;
	kible::themis::FPSRequest request;

	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url;

	#endif

	request.set_fps(fps);

	pb::Initialize_THEMIS_CLIENT(&client_t, url);
	pb::FPS_THEMIS_CLIENT(&client_t, &request);
	pb::Delete_THEMIS_CLIENT(&client_t);
}