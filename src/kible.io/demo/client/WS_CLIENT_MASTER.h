#ifndef WS_CLIENT_MASTER_H_
#define WS_CLIENT_MASTER_H_

#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>
#include "utils.h"
#include "CONCURRENT_QUEUE.h"
#include <sys/time.h>
#include <unistd.h>

#define WEB_SOCKET_POOL_SIZE 80
#define MAX_WEBSOCKET_PACKET_SIZE 100000
#define WEB_SOCKET_SLEEP_TIME 1
#define MAX_HOSTS 20
#define MAX_ACCUMULATED_FRAMES 4
#define WEB_SOCKET_TIME_OUT 10
#define WS_SLEEP_TIME 1

struct WEBSOCKET_ELEMENT {
	int32_t		size;
	uint8_t*	bytes;
};

struct WS_CLIENT_MASTER {
	char name[100];
	volatile uint8_t	accept;
	uint8_t host_count;
	EMSCRIPTEN_WEBSOCKET_T socket;

	Queue<WEBSOCKET_ELEMENT*>	*pool;
	Queue<WEBSOCKET_ELEMENT*>	*active_read[MAX_HOSTS];
};

bool Initialize_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, int, char*);
void Set_Name_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, char*);
bool Set_Recv_Timeout_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, int, int);
bool Set_High_Priority_WS_CLIENT_MASTER(WS_CLIENT_MASTER*);
void Delete_WS_CLIENT_MASTER(WS_CLIENT_MASTER*);

bool Send_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, uint8_t*, uint32_t, uint8_t);
bool Receive_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, uint8_t*, int32_t, int32_t,
	uint8_t);
int Receive_Unsafe_WS_CLIENT_MASTER(WS_CLIENT_MASTER*, uint8_t*, int32_t,
	uint8_t);
uint8_t Register_Vhost_WS_CLIENT_MASTER(WS_CLIENT_MASTER*);

#endif