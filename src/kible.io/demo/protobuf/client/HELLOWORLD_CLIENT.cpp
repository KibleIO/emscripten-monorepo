#include "HELLOWORLD_CLIENT.h"

bool Initialize_HELLOWORLD_CLIENT(HELLOWORLD_CLIENT *client,
	std::string address) {

	return pb::Initialize_HELLOWORLD_CLIENT(&client->client, address);
}

bool Hello_HELLOWORLD_CLIENT(HELLOWORLD_CLIENT *client, std::string str,
	std::string *response_value) {

	gaia::HelloReq request;
	gaia::HelloResp response;

	request.set_subject(str);

	if (!pb::Hello_HELLOWORLD_CLIENT(&client->client, &request,
		&response)) {

		return false;
	}
	*response_value = response.text();
	
	//hacky temporary fix to avoid this function being optimized out
	std::cout << std::endl;

	return true;
}

void Delete_HELLOWORLD_CLIENT(HELLOWORLD_CLIENT *client) {
	pb::Delete_HELLOWORLD_CLIENT(&client->client);
}