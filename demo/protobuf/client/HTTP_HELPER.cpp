#include "HTTP_HELPER.h"

std::string Generate_Request_Address(std::string service_address,
	std::string http_path, std::string package, std::string service_name,
	std::string endpoint_name) {
	
	return service_address + http_path + "/" + package + "." +
		service_name + "/" + endpoint_name;
}