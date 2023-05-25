#ifndef _SERVICE_CLIENT_H_
#define _SERVICE_CLIENT_H_

#include <stdarg.h>
#include "../../Utilities/KCONTEXT.h"
#include "../../Utilities/UTILS.h"
#include "../../Networking/base/client/SOCKET_CLIENT.h"
#include "../../Networking/base/client/SOCKET_CLIENT_REGISTRY.h"

#define MAX_SERVICES 30

struct SERVICE_CLIENT_REGISTRY;

struct SERVICE_CLIENT {
	virtual bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) = 0;
	virtual void Delete() = 0;
};

struct SERVICE_CLIENT_REGISTRY {
	KCONTEXT *ctx;
	SOCKET_CLIENT_REGISTRY *socket_client_registry;
	SERVICE_CLIENT *service_client[MAX_SERVICES];
	int service_count;
};

bool Initialize_SERVICE_CLIENT_REGISTRY(SERVICE_CLIENT_REGISTRY*, KCONTEXT*,
	int count, ...);

template<typename T>
T Get_Instance_Of_SERVICE_CLIENT_REGISTRY(SERVICE_CLIENT_REGISTRY*);


void Delete_SERVICE_CLIENT_REGISTRY(SERVICE_CLIENT_REGISTRY*);

#endif