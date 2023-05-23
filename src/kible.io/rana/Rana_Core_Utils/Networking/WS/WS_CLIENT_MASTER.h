#ifndef WS_CLIENT_MASTER_H_
#define WS_CLIENT_MASTER_H_

#ifdef __EMSCRIPTEN__

#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>

#endif

#include <cstring>
#include "../../Utilities/KCONTEXT.h"
#include "../../Utilities/CONCURRENT_QUEUE.h"
#include "../../Utilities/UTILS.h"
#include "../base/client/RECEIVE_CALLBACK_SOCKET_CLIENT.h"

#define WEB_SOCKET_POOL_SIZE 100
#define MAX_WEBSOCKET_PACKET_SIZE 10000
#define WEB_SOCKET_SLEEP_TIME 1
#define MAX_HOSTS 20
#define MAX_ACCUMULATED_FRAMES 4
#define WEB_SOCKET_TIME_OUT 10
#define WS_SLEEP_TIME 1

struct WEBSOCKET_ELEMENT {
	int32_t		size;
	uint8_t*	bytes;
};

struct WEBSOCKET_CONSUMER {
	void *user_ptr;
	Receive_Callback_SOCKET_CLIENT callback;
};

struct WS_CLIENT_MASTER {
	char name[100];
	volatile uint8_t	accept;
	uint8_t host_count;
	KCONTEXT *ctx;

	thread *websocket_thread;

	WEBSOCKET_CONSUMER *consumers[MAX_HOSTS];
	
	#ifdef __EMSCRIPTEN__

	EMSCRIPTEN_WEBSOCKET_T socket;

	#endif
};

bool Initialize_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, KCONTEXT*, int, char*);
void Set_Name_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, char*);
bool Set_Recv_Timeout_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, int, int);
bool Set_High_Priority_WS_CLIENT_MASTER(WS_CLIENT_MASTER*);
void Delete_WS_CLIENT_MASTER(WS_CLIENT_MASTER*);

bool Send_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, uint8_t*, uint32_t, uint8_t);
uint8_t Register_Vhost_WS_CLIENT_MASTER(WS_CLIENT_MASTER*,
	Receive_Callback_SOCKET_CLIENT, void*);

#endif