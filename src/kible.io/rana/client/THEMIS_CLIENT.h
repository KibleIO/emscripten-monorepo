#ifndef _THEMIS_CLIENT_H_
#define _THEMIS_CLIENT_H_

#include <string>
#include <Utilities/KCONTEXT.h>
#include "../pb/THEMIS_CLIENT.h"
#include "KEEP_ALIVE_CLIENT.h"
#include "../LIMITS.h"

void Launch_THEMIS_CLIENT(KCONTEXT*, HTTP_Protobuf_Callback, void*);
void Ping_THEMIS_CLIENT(KCONTEXT*);
void Density_THEMIS_CLIENT(KCONTEXT*, kible::themis::PixelDensity);
void Dimensions_THEMIS_CLIENT(KCONTEXT*, int, int);
void FPS_THEMIS_CLIENT(KCONTEXT*, int);

#endif