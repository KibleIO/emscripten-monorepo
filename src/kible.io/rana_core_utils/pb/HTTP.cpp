#include "HTTP.h"

void HTTP_Request(char *url, char *data, HTTP_Request_Callback callback,
	void *user_ptr) {

	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "POST");
	attr.attributes = EMSCRIPTEN_FETCH_REPLACE |
		EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.onsuccess = callback;
	attr.onerror = callback;
	attr.userData = user_ptr;
	attr.requestData = data;
	attr.requestDataSize = strlen(attr.requestData);
	const char * headers[] = {"Content-Type", "application/json", 0};
   	attr.requestHeaders = headers;

	emscripten_fetch(&attr, url);
}

void HTTP_Request(char *url, char *data) {
	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "POST");
	attr.attributes = EMSCRIPTEN_FETCH_REPLACE |
		EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.requestData = data;
	attr.requestDataSize = strlen(attr.requestData);
	const char * headers[] = {"Content-Type", "application/json", 0};
   	attr.requestHeaders = headers;

	emscripten_fetch(&attr, url);
}