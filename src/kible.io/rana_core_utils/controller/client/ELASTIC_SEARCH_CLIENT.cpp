#include "ELASTIC_SEARCH_CLIENT.h"

void Post_ELASTIC_SEARCH_CLIENT(std::string url, char *data) {
	HTTP_Request((char*) url.c_str(), data);
}