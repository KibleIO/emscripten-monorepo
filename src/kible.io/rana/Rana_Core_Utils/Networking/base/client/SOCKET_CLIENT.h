#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_

#include "../../WS/WS_CLIENT.h"
#include "../../../Utilities/KCONTEXT.h"
#include "SOCKET_CLIENT_REGISTRY.h"
#include "RECEIVE_CALLBACK_SOCKET_CLIENT.h"

struct SOCKET_CLIENT {
	int type;
	union {
		WS_CLIENT ws_client;
	};
};

bool Initialize_SOCKET_CLIENT(SOCKET_CLIENT*, Receive_Callback_SOCKET_CLIENT,
	SOCKET_CLIENT_REGISTRY*, KCONTEXT*, void*);
//bool Initialize_SOCKET_CLIENT(SOCKET_CLIENT*, Receive_Callback_SOCKET_CLIENT,
//	WS_CLIENT_MASTER*, KCONTEXT*, void*);
bool Send_SOCKET_CLIENT(SOCKET_CLIENT*, char*, int);
void Delete_SOCKET_CLIENT(SOCKET_CLIENT*);

#endif