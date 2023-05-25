#ifndef KEEP_ALIVE_CLIENT_H_
#define KEEP_ALIVE_CLIENT_H_

#include <emscripten.h>
#include <google/protobuf/util/json_util.h>
#include <Utilities/KCONTEXT.h>
#include <Utilities/UTILS.h>
#include <controller/client/SERVICE_CLIENT.h>
#include "THEMIS_CLIENT.h"
#include "../../gen/themis/themis.pb.h"

#define KEEP_ALIVE_CLIENT_CALLBACK_INTERVAL_MS 1000

struct KEEP_ALIVE_CLIENT : public SERVICE_CLIENT {
	KCONTEXT *ctx;

	bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) override;
	void Delete() override;
};

#endif
