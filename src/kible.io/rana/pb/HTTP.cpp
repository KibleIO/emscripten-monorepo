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

void HTTP_Success1(emscripten_fetch_t *fetch) {
	/*
	int characters_to_copy = fetch->numBytes;
	HTTP_SYNC_OBJ *sync_obj = (HTTP_SYNC_OBJ*) fetch->userData;

	if (characters_to_copy > MAX_HTTP_RESPONSE_SIZE) {
		characters_to_copy = MAX_HTTP_RESPONSE_SIZE;
	}

	strncpy(sync_obj->output, fetch->data, characters_to_copy);
  	sync_obj->output[MAX_HTTP_RESPONSE_SIZE - 1] = '\0';

	sync_obj->loop_control = false;
	sync_obj->success = true;
	*/

	emscripten_fetch_close(fetch);
}

void HTTP_Failure1(emscripten_fetch_t *fetch) {
	
	HTTP_SYNC_OBJ *sync_obj = (HTTP_SYNC_OBJ*) fetch->userData;
	
	std::cout << "in failure " << (void*) fetch->userData << std::endl;
	sync_obj->loop_control = false;
	//sync_obj->success = false;
	
	

	emscripten_fetch_close(fetch);
}

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
	attr.requestHeaders = JSON_CONTENT_TYPE;

	emscripten_fetch(&attr, url);
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