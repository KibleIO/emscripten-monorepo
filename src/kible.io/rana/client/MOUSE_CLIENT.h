#ifndef MOUSE_CLIENT_H_
#define MOUSE_CLIENT_H_

#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/controller/client/SERVICE_CLIENT.h"
#include "../Rana_Core_Utils/Utilities/MOUSE.h"
#include "../Rana_Core_Utils/Utilities/UTILS.h"

struct MOUSE_CLIENT : public SERVICE_CLIENT {
	KCONTEXT *ctx;

	SOCKET_CLIENT socket_client;

	bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) override;
	void Delete() override;
};

void Send_Event_MOUSE_CLIENT(MOUSE_CLIENT*, MOUSE_EVENT_T*);

#endif
