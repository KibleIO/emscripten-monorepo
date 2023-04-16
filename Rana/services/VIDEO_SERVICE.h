#ifndef VIDEO_SERVICE_H_
#define VIDEO_SERVICE_H_

#include <SDL2/SDL.h>
#include <emscripten.h>

#include <thread>

#include "../LIMITS.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"
#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Utilities/MOUSE.h"
#include "../Rana_Core_Utils/Utilities/KEYBOARD.h"
#include "../Rana_Core_Utils/Utilities/TIMER.h"
#include "../Broadway-H.264-decoder/src/Decoder.h"
#include "MOUSE_SERVICE.h"
#include "KEYBOARD_SERVICE.h"

// god have mercy on us
#define MAX_NAL_SIZE 100000

#define MINIMUM_WIDTH 480
#define MINIMUM_HEIGHT 480

#define MAXIMUM_WIDTH 1920
#define MAXIMUM_HEIGHT 1080

struct VIDEO_SERVICE {
	KCONTEXT *ctx;

	CLIENT *c;
	thread *main_loop;
	volatile bool main_loop_running;
	char nal_buffer[MAX_NAL_SIZE];

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;

	Decoder decoder;
	u8 *byteStrmStart;
	u32 strmLen;

	int stop = 0;
	int counter = 0;
	int mouse_count;
	int keyboard_count;

	int width, height;

	float x_scale;
	float y_scale;

	MOUSE_SERVICE *mouse_service;
	KEYBOARD_SERVICE *keyboard_service;
};

bool Initialize_VIDEO_SERVICE(VIDEO_SERVICE *, KCONTEXT *, MOUSE_SERVICE *, KEYBOARD_SERVICE *);
void Main_TCP_Loop_VIDEO_SERVICE(void *);
void Main_UDP_Loop_VIDEO_SERVICE(VIDEO_SERVICE *);
bool Connect_VIDEO_SERVICE(VIDEO_SERVICE *, CLIENT *, CLIENT *);
void Get_Frame_VIDEO_SERVICE(VIDEO_SERVICE *, uint8_t *);
bool Status_VIDEO_SERVICE(VIDEO_SERVICE *);
bool Resize_VIDEO_SERVICE(VIDEO_SERVICE *, int, int);
void Disconnect_VIDEO_SERVICE(VIDEO_SERVICE *);
void Delete_VIDEO_SERVICE(VIDEO_SERVICE *);

#endif
