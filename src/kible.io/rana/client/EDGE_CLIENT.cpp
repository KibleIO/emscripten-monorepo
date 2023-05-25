#include "EDGE_CLIENT.h"

void Themis_EDGE_CLIENT(std::string address, std::string session_id,
	std::string* themis_url, HTTP_Protobuf_Callback callback,
	void *user_data) {
	
	pb::EDGE_CLIENT client;
	kible::edge::ThemisRequest request;

	request.mutable_sessionid()->mutable_uuid()->set_value(session_id);

	pb::Initialize_EDGE_CLIENT(&client, address);
	pb::Themis_EDGE_CLIENT(&client, &request, callback, user_data);
	pb::Delete_EDGE_CLIENT(&client);
}