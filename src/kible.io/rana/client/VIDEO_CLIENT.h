#ifndef VIDEO_CLIENT_H_
#define VIDEO_CLIENT_H_

#include <SDL2/SDL.h>
#include <emscripten.h>

#include <thread>
#include "../Rana_Core_Utils/controller/client/SERVICE_CLIENT.h"
#include "../LIMITS.h"
#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Utilities/MOUSE.h"
#include "../Rana_Core_Utils/Utilities/KEYBOARD.h"
#include "../Rana_Core_Utils/Utilities/TIMER.h"
#include "../Rana_Core_Utils/Utilities/CONCURRENT_QUEUE.h"
#include "../../Broadway-H.264-decoder/src/Decoder.h"
#include "MOUSE_CLIENT.h"
#include "KEYBOARD_CLIENT.h"
#include "../client/THEMIS_CLIENT.h"

// god have mercy on us
#define MAX_NAL_SIZE 500000

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

struct VIDEO_ELEMENT {
	int32_t		size;
	uint8_t*	bytes;
};

struct VIDEO_CLIENT : public SERVICE_CLIENT {
	KCONTEXT *ctx;

	//thread *main_loop;
	volatile bool main_loop_running;
	//char nal_buffer[MAX_NAL_SIZE];

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
	bool ctrl_clicked;
	bool relative_mode;

	MOUSE_CLIENT *mouse;
	KEYBOARD_CLIENT *keyboard;

	SOCKET_CLIENT socket_client;
	Queue<VIDEO_ELEMENT*>	*pool;

	bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) override;
	void Delete() override;
};

void Main_TCP_Loop_VIDEO_CLIENT(void *);
void Main_UDP_Loop_VIDEO_CLIENT(VIDEO_CLIENT *);
void Get_Frame_VIDEO_CLIENT(VIDEO_CLIENT *, uint8_t *);
bool Status_VIDEO_CLIENT(VIDEO_CLIENT *);
bool Resize_VIDEO_CLIENT(VIDEO_CLIENT *, int, int);
void Disconnect_VIDEO_CLIENT(VIDEO_CLIENT *);

//void Recv_Callback_VIDEO_CLIENT(void *, char *, int);

#endif
