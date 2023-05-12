#ifndef _HTTP_H_
#define _HTTP_H_

#include <emscripten/fetch.h>
#include <stdio.h>
#include <string.h>

#define MAX_HTTP_RESPONSE_SIZE 256
#define HTTP_TIMEOUT 100
#define HTTP_WAIT_MS 100
#define JSON_CONTENT_TYPE (const char*[])\
{ "Content-Type", "application/json", 0 }

struct HTTP_SYNC_OBJ {
	bool loop_control;
	bool success;
	char *output;
};

bool HTTP_Request(char*, char*, char*, char*);
bool HTTP_Request_With_Header(char*, char*, char*, char*, const char**);

#endif