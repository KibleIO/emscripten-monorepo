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

#define UUID_STR_SIZE 37
#define CORE_SYSTEM_STR_SIZE 37
#define CURRENT_TIME_LEN 100
#define MAX_COOKIE_SIZE 256
#define MINIMUM_WIDTH 480
#define MINIMUM_HEIGHT 480
#define MAXIMUM_WIDTH 1920
#define MAXIMUM_HEIGHT 1080

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <emscripten/emscripten.h>

struct s_mallinfo {
	int arena;    /* non-mmapped space allocated from system */
	int ordblks;  /* number of free chunks */
	int smblks;   /* always 0 */
	int hblks;    /* always 0 */
	int hblkhd;   /* space in mmapped regions */
	int usmblks;  /* maximum total allocated space */
	int fsmblks;  /* always 0 */
	int uordblks; /* total allocated space */
	int fordblks; /* total free space */
	int keepcost; /* releasable (via malloc_trim) space */
};

extern "C" {
	extern s_mallinfo mallinfo();
}

void Sleep_Milli(unsigned int milli);

void log_dbg(char *hello);

void log_err(char *hello);

void get_screen_width_height(int *width, int *height);

void Get_Cookie(char*);

void Get_Url(char *);

int64_t getTime();

unsigned int getTotalMemory();

unsigned int getFreeMemory();

void get_current_time(char*);

void generate_uuid(char*);

#endif