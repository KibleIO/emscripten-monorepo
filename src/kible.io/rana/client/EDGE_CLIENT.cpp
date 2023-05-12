#include "EDGE_CLIENT.h"

bool Themis_EDGE_CLIENT(std::string session_id, std::string* themis_url) {
	pb::EDGE_CLIENT client;
	bool return_value;

	pb::Initialize_EDGE_CLIENT(&client, "http://localhost:8080");

	kible::edge::ThemisRequest request;
	kible::edge::ThemisResponse response;

	request.mutable_sessionid()->mutable_uuid()->set_value(session_id);

	return_value = pb::Themis_EDGE_CLIENT(&client, &request,
		&response);

	if (return_value) {
		*themis_url = response.address();
	}
	
	pb::Delete_EDGE_CLIENT(&client);

	return return_value;
}