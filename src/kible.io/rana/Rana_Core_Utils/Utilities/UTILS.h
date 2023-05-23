#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <iostream>
#include <unistd.h>
#include <emscripten/websocket.h>
#include <emscripten/emscripten.h>
#include <thread>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

#define MAX_COOKIE_SIZE 256
#define MINIMUM_WIDTH 480
#define MINIMUM_HEIGHT 480

#define MAXIMUM_WIDTH 1920
#define MAXIMUM_HEIGHT 1080

void Sleep_Milli(unsigned int milli);

void log_dbg(char *hello);

void log_err(char *hello);

void get_screen_width_height(int *width, int *height);

void Get_Cookie(char*);

void Get_Url(char *);

int64_t getTime();

#endif