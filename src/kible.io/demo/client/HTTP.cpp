#include "HTTP.h"

void HTTP_Success(emscripten_fetch_t *fetch) {
	int characters_to_copy = fetch->numBytes;
	HTTP_SYNC_OBJ *sync_obj = (HTTP_SYNC_OBJ*) fetch->userData;

	if (characters_to_copy > MAX_HTTP_RESPONSE_SIZE) {
		characters_to_copy = MAX_HTTP_RESPONSE_SIZE;
	}

	strncpy(sync_obj->output, fetch->data, characters_to_copy);
  	sync_obj->output[MAX_HTTP_RESPONSE_SIZE - 1] = '\0';
	emscripten_fetch_close(fetch);

	sync_obj->loop_control = false;
	sync_obj->success = true;
}

void HTTP_Failure(emscripten_fetch_t *fetch) {
	HTTP_SYNC_OBJ *sync_obj = (HTTP_SYNC_OBJ*) fetch->userData;
	emscripten_fetch_close(fetch);

	sync_obj->loop_control = false;
	sync_obj->success = false;
}

bool HTTP_Request(char *url, char *type, char *data, char *output) {
	return HTTP_Request_With_Header(url, type, data, output,
		JSON_CONTENT_TYPE);
}

bool HTTP_Request_With_Header(char *url, char *type, char *data, char *output,
	const char **headers) {
	
	bool return_value;
	int counter = HTTP_TIMEOUT;

	HTTP_SYNC_OBJ *sync_obj = new HTTP_SYNC_OBJ;
	sync_obj->loop_control = true;
	sync_obj->success = false;
	sync_obj->output = output;

	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, type);
	attr.attributes = EMSCRIPTEN_FETCH_REPLACE |
		EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	
	attr.requestHeaders = headers;

	attr.requestData = data;
	attr.requestDataSize = strlen(attr.requestData);

	attr.onsuccess = HTTP_Success;
	attr.onerror = HTTP_Failure;

	attr.userData = (void*) sync_obj;

	emscripten_fetch(&attr, url);

	while (sync_obj->loop_control && counter-- > 0) {
		emscripten_sleep(HTTP_WAIT_MS);
	}

	return_value = sync_obj->success;
	delete sync_obj;

	return (counter > 0) && return_value;
}