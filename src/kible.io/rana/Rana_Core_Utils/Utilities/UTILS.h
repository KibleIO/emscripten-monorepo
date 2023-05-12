#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <iostream>
#include <unistd.h>
#include <emscripten/websocket.h>
#include <emscripten/emscripten.h>
#include <thread>
#include <string.h>

using namespace std;

#define MAX_COOKIE_SIZE 256

void Sleep_Milli(unsigned int milli);

void log_dbg(char *hello);

void log_err(char *hello);

void get_screen_width_height(int *width, int *height);

void get_cookie(char *str);

#endif