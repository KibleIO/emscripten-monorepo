#include "THEMIS_CLIENT.h"

bool Launch_THEMIS_CLIENT(KCONTEXT *ctx) {
	pb::THEMIS_CLIENT client_t;
	bool return_value;
	int width;
	int height;
	std::string url;

	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#endif

	pb::Initialize_THEMIS_CLIENT(&client_t, url);

	kible::themis::LaunchRequest request;
	kible::themis::LaunchResponse response;

	get_screen_width_height(&width, &height);
	Set_Screen_Dim_KCONTEXT(ctx, (SCREEN_DIM) {
		width,
		width,
		height});
	
	request.set_width(width);
	request.set_height(height);
	request.set_launchbackend(
		kible::themis::LaunchBackend::LAUNCHBACKEND_WS);

	return_value = pb::Launch_THEMIS_CLIENT(&client_t, &request,
		&response);
	
	pb::Delete_THEMIS_CLIENT(&client_t);

	return return_value;
}



void Ping_THEMIS_CLIENT(KCONTEXT *ctx, void *keep) {
	//char *out = new char[MAX_HTTP_RESPONSE_SIZE];
	//HTTP_Request("http://localhost:4461/api/kible.themis.THEMIS/Ping", "POST", "{}", out);
	//delete out;

	//HTTP_Request_Temp(generic_stuff_iguess, keep);
	/*
	pb::THEMIS_CLIENT *client_t = new pb::THEMIS_CLIENT;
	std::string url;

	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#endif

	std::cout << "ok, here's the url " << url << std::endl;

	
	pb::Initialize_THEMIS_CLIENT(client_t, url);

	kible::themis::PingRequest *request = new kible::themis::PingRequest;
	kible::themis::PingResponse *response = new kible::themis::PingResponse;
	
	pb::Ping_THEMIS_CLIENT(client_t, request,
		response);
	
	pb::Delete_THEMIS_CLIENT(&client_t);
	*/
}

bool Density_THEMIS_CLIENT(KCONTEXT *ctx,
	kible::themis::PixelDensity density) {
	
	pb::THEMIS_CLIENT client_t;
	bool return_value;
	std::string url;

	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url + ":" + std::to_string(ctx->http_services_backbone_port);

	#endif

	pb::Initialize_THEMIS_CLIENT(&client_t, url);

	kible::themis::DensityRequest request;
	kible::themis::DensityResponse response;

	request.set_density(density);

	return_value = pb::Density_THEMIS_CLIENT(&client_t, &request,
		&response);
	
	pb::Delete_THEMIS_CLIENT(&client_t);

	return return_value;
}