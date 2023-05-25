#ifndef KEYBOARD_CLIENT_H_
#define KEYBOARD_CLIENT_H_

#include <Utilities/ASSERT.h>
#include <Utilities/KCONTEXT.h>
#include <controller/client/SERVICE_CLIENT.h>
#include <Utilities/KEYBOARD.h>

struct KEYBOARD_CLIENT : public SERVICE_CLIENT {
	KCONTEXT *ctx;

	SOCKET_CLIENT socket_client;

	bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) override;
	void Delete() override;
};

void Send_Event_KEYBOARD_CLIENT(KEYBOARD_CLIENT*, KEYBOARD_EVENT_T*);

#endif
