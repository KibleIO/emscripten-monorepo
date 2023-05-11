#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <unistd.h>
#include <emscripten/websocket.h>
#include <emscripten/emscripten.h>

#include <thread>

using namespace std;

void Sleep_Milli(unsigned int milli);

void log_dbg(char *hello);

void log_err(char *hello);

void get_screen_width_height(int *width, int *height);

#endif