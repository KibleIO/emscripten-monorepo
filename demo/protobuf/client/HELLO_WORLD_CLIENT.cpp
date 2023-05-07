#include "HELLO_WORLD_CLIENT.h"

bool Initialize_HELLO_WORLD_CLIENT(HELLO_WORLD_CLIENT *client,
	std::string service_address) {
	
  	client->service_address = service_address;

	return true;
}

void Delete_HELLO_WORLD_CLIENT(HELLO_WORLD_CLIENT *client) {
	//nop
}

bool Hello_HELLO_WORLD_CLIENT(HELLO_WORLD_CLIENT *client,
	gaia::HelloReq *request, gaia::HelloResp *response) {

	std::string json_string;

	google::protobuf::util::JsonPrintOptions options;
	options.add_whitespace = false;
	options.always_print_primitive_fields = true;
	MessageToJsonString(*request, &json_string, options);

	char response_raw[MAX_HTTP_RESPONSE_SIZE];

	std::string request_address = Generate_Request_Address(
		client->service_address, HELLO_WORLD_CLIENT_HTTP_PATH,
		HELLO_WORLD_CLIENT_PACKAGE, HELLO_WORLD_CLIENT_SERVICE_NAME,
		HELLO_WORLD_CLIENT_HELLO);

	if (!HTTP_Request((char*) request_address.c_str(), "POST",
		(char*) json_string.c_str(),
		response_raw)) {

		//the http request failed for one reason or another
		return false;
	}

	google::protobuf::util::JsonParseOptions options2;
	JsonStringToMessage(std::string(response_raw), response, options2);

	return true;
}