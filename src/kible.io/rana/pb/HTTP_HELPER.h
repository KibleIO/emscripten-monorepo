#ifndef _HTTP_HELPER_H_
#define _HTTP_HELPER_H_

#include <google/protobuf/util/json_util.h>
#include <string>
#include "HTTP.h"

#define HTTP_Protobuf_Init_Delete(package_actual, service_actual_name,\
	path_actual)\
	bool pb::Initialize_##service_actual_name##_CLIENT(\
		pb::service_actual_name##_CLIENT *client,\
		std::string service_address) {\
  	client->service_address = service_address;\
	client->service_package = #package_actual;\
	client->service_name = #service_actual_name;\
	client->service_path = #path_actual;\
	return true;\
}\
void pb::Delete_##service_actual_name##_CLIENT(\
	pb::service_actual_name##_CLIENT *client) {}\

#define HTTP_Protobuf_Begin(service_actual_name)\
	namespace pb {\
	struct service_actual_name##_CLIENT {\
		std::string service_address;\
		std::string service_package;\
		std::string service_name;\
		std::string service_path;\
	};\
	bool Initialize_##service_actual_name##_CLIENT(\
		service_actual_name##_CLIENT*, std::string);\
	void Delete_##service_actual_name##_CLIENT(\
		service_actual_name##_CLIENT*);\

#define HTTP_Protobuf_End()\
	};\

#define HTTP_Protobuf_Endpoint(service_actual_name, endpoint_name,\
	request, response)\
	bool endpoint_name##_##service_actual_name##_CLIENT(\
		service_actual_name##_CLIENT*, request*, response*);\


#define HTTP_Protobuf_Endpoint_Definition(service_actual_name, endpoint_name,\
	request_obj, response_obj)\
	bool pb::endpoint_name##_##service_actual_name##_CLIENT(pb::service_actual_name##_CLIENT *client,\
		request_obj *request, response_obj *response) {\
		std::string json_string;\
		google::protobuf::util::JsonPrintOptions options;\
		options.add_whitespace = false;\
		options.always_print_primitive_fields = true;\
		MessageToJsonString(*request, &json_string, options);\
		char response_raw[MAX_HTTP_RESPONSE_SIZE];\
		std::string request_address = client->service_address + \
			client->service_path + "/" + \
			client->service_package + "." + client->service_name + \
			"/" + #endpoint_name;\
		if (!HTTP_Request((char*) request_address.c_str(), "POST",\
			(char*) json_string.c_str(),\
			response_raw)) {\
			return false;\
		}\
		google::protobuf::util::JsonParseOptions options2;\
		JsonStringToMessage(std::string(response_raw), response, options2);\
		return true;\
	}\

#endif