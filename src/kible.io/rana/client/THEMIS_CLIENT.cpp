#include "THEMIS_CLIENT.h"

bool THEMIS_CLIENT::Initialize(KCONTEXT *ctx, SERVICE_CLIENT_REGISTRY *registry) {
	#ifdef TESTING_BUILD

	url = std::string("http://") + ctx->themis_url + std::to_string(ctx->http_services_backbone_port);

	#else

	url = std::string("https://") + ctx->themis_url + std::to_string(ctx->http_services_backbone_port);

	#endif

	return Launch_THEMIS_CLIENT(this);
}

void THEMIS_CLIENT::Delete() {}

bool Launch_THEMIS_CLIENT(THEMIS_CLIENT *client) {
	pb::THEMIS_CLIENT client_t;
	bool return_value;

	pb::Initialize_THEMIS_CLIENT(&client_t, client->url);

	kible::themis::LaunchRequest request;
	kible::themis::LaunchResponse response;

	return_value = pb::Launch_THEMIS_CLIENT(&client_t, &request,
		&response);
	
	pb::Delete_THEMIS_CLIENT(&client_t);

	return return_value;
}

bool Density_THEMIS_CLIENT(THEMIS_CLIENT *client,
	kible::themis::PixelDensity density) {
	
	pb::THEMIS_CLIENT client_t;
	bool return_value;

	pb::Initialize_THEMIS_CLIENT(&client_t, client->url);

	kible::themis::DensityRequest request;
	kible::themis::DensityResponse response;

	request.set_density(density);

	return_value = pb::Density_THEMIS_CLIENT(&client_t, &request,
		&response);
	
	pb::Delete_THEMIS_CLIENT(&client_t);

	return return_value;
}