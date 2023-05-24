#ifndef KEEP_ALIVE_CLIENT_H_
#define KEEP_ALIVE_CLIENT_H_

#include <emscripten.h>
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/controller/client/SERVICE_CLIENT.h"
#include "../pb/HTTP.h"

#define KEEP_ALIVE_CLIENT_CALLBACK_INTERVAL_MS 1000

struct KEEP_ALIVE_CLIENT : public SERVICE_CLIENT {
	KCONTEXT *ctx;
	bool testing;

	bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) override;
	void Delete() override;
};

#endif
