#ifndef _THEMIS_CLIENT_H_
#define _THEMIS_CLIENT_H_

#include <string>
#include "../pb/THEMIS_CLIENT.h"

bool Launch_THEMIS_CLIENT(std::string);
bool Density_THEMIS_CLIENT(std::string, kible::themis::PixelDensity);

#endif