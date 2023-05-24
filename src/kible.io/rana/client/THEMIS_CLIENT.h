#ifndef _THEMIS_CLIENT_H_
#define _THEMIS_CLIENT_H_

#include <string>
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../pb/THEMIS_CLIENT.h"
#include "KEEP_ALIVE_CLIENT.h"

bool Launch_THEMIS_CLIENT(KCONTEXT*);
void Ping_THEMIS_CLIENT(KCONTEXT*, void*);
bool Density_THEMIS_CLIENT(KCONTEXT*, kible::themis::PixelDensity);

#endif