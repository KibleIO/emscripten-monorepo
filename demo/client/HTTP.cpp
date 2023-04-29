#include "HTTP.h"

void success_http(emscripten_fetch_t *fetch) {
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

void failure_http(emscripten_fetch_t *fetch) {
	HTTP_SYNC_OBJ *sync_obj = (HTTP_SYNC_OBJ*) fetch->userData;
	emscripten_fetch_close(fetch);

	sync_obj->loop_control = false;
	sync_obj->success = false;
}

bool issue_request(char *url, char *type, char *data, char *output) {
	bool return_value;
	int counter = HTTP_TIMEOUT;

	HTTP_SYNC_OBJ *sync_obj = new HTTP_SYNC_OBJ;
	sync_obj->loop_control = true;
	sync_obj->success = false;
	sync_obj->output = output;

	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, type);
	attr.attributes = EMSCRIPTEN_FETCH_REPLACE | EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	
	const char* headers[] = { "Content-Type", "application/json", 0 };
	attr.requestHeaders = headers;

	attr.requestData = data;
	attr.requestDataSize = strlen(attr.requestData);

	attr.onsuccess = success_http;
	attr.onerror = failure_http;

	attr.userData = (void*) sync_obj;

	emscripten_fetch(&attr, url);

	while (sync_obj->loop_control && counter-- > 0) {
		emscripten_sleep(HTTP_WAIT_MS);
	}

	return_value = sync_obj->success;
	delete sync_obj;

	return (counter > 0) && return_value;
}