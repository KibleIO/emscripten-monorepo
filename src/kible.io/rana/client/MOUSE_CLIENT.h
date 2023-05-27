#ifndef MOUSE_CLIENT_H_
#define MOUSE_CLIENT_H_

#include <Utilities/ASSERT.h>
#include <Utilities/KCONTEXT.h>
#include <controller/client/SERVICE_CLIENT.h>
#include <Utilities/MOUSE.h>

struct MOUSE_CLIENT : public SERVICE_CLIENT {
	KCONTEXT *ctx;

	SOCKET_CLIENT socket_client;

	bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) override;
	void Delete() override;
};

void Send_Event_MOUSE_CLIENT(MOUSE_CLIENT*, MOUSE_EVENT_T*);

#endif
