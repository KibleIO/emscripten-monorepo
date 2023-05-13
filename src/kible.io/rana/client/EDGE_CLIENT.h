#ifndef _EDGE_CLIENT_H_
#define _EDGE_CLIENT_H_

#include <string>
#include "../pb/EDGE_CLIENT.h"

#define EDGE_SERVER_ADDRESS "http://localhost:8080"

bool Themis_EDGE_CLIENT(std::string, std::string*);

#endif