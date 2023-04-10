#ifndef VIDEO_SERVICE_H_
#define VIDEO_SERVICE_H_

#include <thread>
#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"
#include "../LIMITS.h"

//god have mercy on us
#define MAX_NAL_SIZE 100000

struct VIDEO_SERVICE {
	KCONTEXT *ctx;
	
	CLIENT *c;
	thread *main_loop;
	volatile bool main_loop_running;
	char nal_buffer[MAX_NAL_SIZE];
};

bool Initialize_VIDEO_SERVICE(VIDEO_SERVICE*, KCONTEXT*);
void Main_TCP_Loop_VIDEO_SERVICE(VIDEO_SERVICE*);
void Main_UDP_Loop_VIDEO_SERVICE(VIDEO_SERVICE*);
bool Connect_VIDEO_SERVICE(VIDEO_SERVICE*, CLIENT*, CLIENT*);
void Get_Frame_VIDEO_SERVICE(VIDEO_SERVICE*, uint8_t*);
bool Status_VIDEO_SERVICE(VIDEO_SERVICE*);
bool Resize_VIDEO_SERVICE(VIDEO_SERVICE*, int, int);
void Disconnect_VIDEO_SERVICE(VIDEO_SERVICE*);
void Delete_VIDEO_SERVICE(VIDEO_SERVICE*);

#endif
