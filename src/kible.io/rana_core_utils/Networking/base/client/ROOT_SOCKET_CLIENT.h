#ifndef _ROOT_SOCKET_CLIENT_H_
#define _ROOT_SOCKET_CLIENT_H_

#include "../../WS/WS_CLIENT_MASTER.h"

struct ROOT_SOCKET_CLIENT {
	int type;
	union {
		WS_CLIENT_MASTER *ws_client_master;
	};
};

bool Initialize_ROOT_SOCKET_CLIENT(ROOT_SOCKET_CLIENT*, KCONTEXT*, int, char*);
void Delete_ROOT_SOCKET_CLIENT(ROOT_SOCKET_CLIENT*);

#endif