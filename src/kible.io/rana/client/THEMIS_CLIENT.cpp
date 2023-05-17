#include "THEMIS_CLIENT.h"

bool Launch_THEMIS_CLIENT(std::string address) {
	
	pb::THEMIS_CLIENT client;
	bool return_value;

	pb::Initialize_THEMIS_CLIENT(&client, address);

	kible::themis::LaunchRequest request;
	kible::themis::LaunchResponse response;

	return_value = pb::Launch_THEMIS_CLIENT(&client, &request,
		&response);
	
	pb::Delete_THEMIS_CLIENT(&client);

	return return_value;
}

bool Density_THEMIS_CLIENT(std::string address,
	kible::themis::PixelDensity density) {
	
	pb::THEMIS_CLIENT client;
	bool return_value;

	pb::Initialize_THEMIS_CLIENT(&client, address);

	kible::themis::DensityRequest request;
	kible::themis::DensityResponse response;

	request.set_density(density);

	return_value = pb::Density_THEMIS_CLIENT(&client, &request,
		&response);
	
	pb::Delete_THEMIS_CLIENT(&client);

	return return_value;
}