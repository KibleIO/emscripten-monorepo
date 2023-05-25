#ifndef WS_CLIENT_H_
#define WS_CLIENT_H_

#include <cstring>
#include "../../Utilities/KCONTEXT.h"
#include "WS_CLIENT_MASTER.h"
#include "../base/client/RECEIVE_CALLBACK_SOCKET_CLIENT.h"
#include "../base/client/SOCKET_CLIENT_REGISTRY.h"

#define TEST_BUFF_SIZE 4
#define DEFAULT_RECV_TIMEOUT 5

struct WS_CLIENT {
	char name[100];
	WS_CLIENT_MASTER *ws_master;
	KCONTEXT *ctx;
	int recv_timeout;
	uint8_t client_id;
};

bool Initialize_WS_CLIENT(WS_CLIENT*, Receive_Callback_SOCKET_CLIENT,
	SOCKET_CLIENT_REGISTRY*, KCONTEXT*, void*);
//bool Initialize_WS_CLIENT(WS_CLIENT*, Receive_Callback_SOCKET_CLIENT,
//	WS_CLIENT_MASTER*, KCONTEXT*, void*);
void Set_Name_WS_CLIENT(WS_CLIENT*, char*);
bool Set_Recv_Timeout_WS_CLIENT(WS_CLIENT*, int, int);
bool Set_High_Priority_WS_CLIENT(WS_CLIENT*);
bool Connect_WS_CLIENT(WS_CLIENT*);
bool Send_WS_CLIENT(WS_CLIENT*, char*, int);
void Delete_WS_CLIENT(WS_CLIENT*);

#endif