#ifndef _THEMIS_CLIENT_H_
#define _THEMIS_CLIENT_H_

#include <string>
#include "../Rana_Core_Utils/controller/client/SERVICE_CLIENT.h"
#include "../pb/THEMIS_CLIENT.h"

struct THEMIS_CLIENT : public SERVICE_CLIENT {
	std::string url;

	bool Initialize(KCONTEXT*, SERVICE_CLIENT_REGISTRY*) override;
	void Delete() override;
};

bool Launch_THEMIS_CLIENT(THEMIS_CLIENT*);
bool Density_THEMIS_CLIENT(THEMIS_CLIENT*, kible::themis::PixelDensity);

#endif