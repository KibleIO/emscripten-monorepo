#ifndef _HELLOWORLD_CLIENT_H_
#define _HELLOWORLD_CLIENT_H_

#include <iostream>
#include <string>
#include "../pb/HELLOWORLD_CLIENT.h"

struct HELLOWORLD_CLIENT {
	pb::HELLOWORLD_CLIENT client;
};

bool Initialize_HELLOWORLD_CLIENT(HELLOWORLD_CLIENT*, std::string);
bool Hello_HELLOWORLD_CLIENT(HELLOWORLD_CLIENT*, std::string, std::string*);
void Delete_HELLOWORLD_CLIENT(HELLOWORLD_CLIENT*);

#endif