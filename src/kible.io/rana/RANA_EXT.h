#ifndef RANA_EXT_H_
#define RANA_EXT_H_

#include <Utilities/KCONTEXT.h>
#include "client/THEMIS_CLIENT.h"
#include "client/VIDEO_CLIENT.h"
#include "client/MOUSE_CLIENT.h"
#include "client/KEYBOARD_CLIENT.h"
#include "client/AUDIO_CLIENT.h"
#include "client/KEEP_ALIVE_CLIENT.h"
#include "LIMITS.h"
#include "utils/UTILS.h"

struct RANA_EXT {
	SERVICE_CLIENT_REGISTRY registry;
};

bool Initialize_RANA_EXT(RANA_EXT*, KCONTEXT*);

#endif
