#ifndef _HTTP_HELPER_H_
#define _HTTP_HELPER_H_

#include <google/protobuf/util/json_util.h>
#include <string>
#include "HTTP.h"

typedef void (*HTTP_Protobuf_Callback)(google::protobuf::Message*, void*);

struct CALLBACK_WRAPPER {
	HTTP_Protobuf_Callback callback;
	void *user_data;
};

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
	void endpoint_name##_##service_actual_name##_CLIENT(\
		service_actual_name##_CLIENT*, request*, \
		HTTP_Protobuf_Callback, void*);\
	void endpoint_name##_##service_actual_name##_CLIENT(\
		service_actual_name##_CLIENT*, request*);\

#define HTTP_Protobuf_Endpoint_Definition(service_actual_name, endpoint_name,\
	request_obj, response_obj)\
	void endpoint_name##_##service_actual_name##_CLIENT_CALLBACK(\
		emscripten_fetch_t *fetch) {\
		CALLBACK_WRAPPER *client = (CALLBACK_WRAPPER*) fetch->userData;\
		if (fetch->status == HTTP_SUCCESS_STATUS) {\
			response_obj response;\
			google::protobuf::util::JsonParseOptions options;\
			JsonStringToMessage(std::string(fetch->data), \
			&response, options);\
			client->callback(&response, client->user_data);\
		} else {\
			client->callback(NULL, client->user_data);\
		}\
		delete client;\
		emscripten_fetch_close(fetch);\
	}\
	void pb::endpoint_name##_##service_actual_name##_CLIENT(\
		pb::service_actual_name##_CLIENT *client, request_obj *request,\
		HTTP_Protobuf_Callback callback_func, void *user_data) {\
		CALLBACK_WRAPPER *callback = new CALLBACK_WRAPPER;\
		callback->callback = callback_func;\
		callback->user_data = user_data;\
		std::string json_string;\
		google::protobuf::util::JsonPrintOptions options;\
		options.add_whitespace = false;\
		options.always_print_primitive_fields = true;\
		MessageToJsonString(*request, &json_string, options);\
		std::string request_address = client->service_address + \
			client->service_path + "/" + \
			client->service_package + "." + client->service_name + \
			"/" + #endpoint_name;\
		HTTP_Request((char*) request_address.c_str(),\
			(char*) json_string.c_str(),\
			endpoint_name##_##service_actual_name##_CLIENT_CALLBACK,\
			(void*)callback);\
	}\
	void pb::endpoint_name##_##service_actual_name##_CLIENT(\
		pb::service_actual_name##_CLIENT *client, request_obj *request) {\
		std::string json_string;\
		google::protobuf::util::JsonPrintOptions options;\
		options.add_whitespace = false;\
		options.always_print_primitive_fields = true;\
		MessageToJsonString(*request, &json_string, options);\
		std::string request_address = client->service_address + \
			client->service_path + "/" + \
			client->service_package + "." + client->service_name + \
			"/" + #endpoint_name;\
		HTTP_Request((char*) request_address.c_str(),\
			(char*) json_string.c_str());\
	}\

#endif