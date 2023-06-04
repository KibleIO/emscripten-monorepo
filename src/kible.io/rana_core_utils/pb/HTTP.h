#ifndef _HTTP_H_
#define _HTTP_H_

#include <emscripten/fetch.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#define HTTP_SUCCESS_STATUS 200

typedef void (*HTTP_Request_Callback)(struct emscripten_fetch_t *fetch);

void HTTP_Request(char*, char*);
void HTTP_Request(char*, char*, HTTP_Request_Callback, void*);

#endif