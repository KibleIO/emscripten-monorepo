#ifndef  LOGGING_H_
#define  LOGGING_H_

#include <string>
#include <iostream>
#include "../../json/json.hpp"
#include "UTILS.h"

struct LOGGING {
	std::string core_system;
	std::string trace_uuid;
	std::string uuid;
};

bool Initialize_LOGGING(LOGGING*, std::string, std::string, std::string);
void Log_LOGGING(LOGGING*, nlohmann::json, char*, char*, int, const char*);
void Delete_LOGGING(LOGGING*);

#endif
