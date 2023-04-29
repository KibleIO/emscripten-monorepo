#ifndef _HTTP_H_
#define _HTTP_H_

#include <emscripten/fetch.h>
#include <stdio.h>
#include <string.h>

#define MAX_HTTP_RESPONSE_SIZE 256
#define HTTP_TIMEOUT 100
#define HTTP_WAIT_MS 100

struct HTTP_SYNC_OBJ {
	bool loop_control;
	bool success;
	char *output;
};

bool HTTP_Request(char *url, char *type, char *data, char *output);

#endif