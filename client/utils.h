#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <unistd.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

using namespace std;

void Sleep_Milli(unsigned int milli);

void log_dbg(char *hello);

void log_err(char *hello);

#endif