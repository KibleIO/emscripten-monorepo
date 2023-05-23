#ifndef MOUSE_CLIENT_H_
#define MOUSE_CLIENT_H_

#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/controller/client/SERVICE_CLIENT.h"

struct MOUSE_CLIENT : public SERVICE_CLIENT {
	KCONTEXT *ctx;

	SOCKET_CLIENT socket_client;

	bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) override;
	void Delete() override;
};

#endif
