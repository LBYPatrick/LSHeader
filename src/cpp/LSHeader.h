#pragma once

#include <string>
#include "ConfigContainer.h"

namespace LSHeader {
	
	string getHeader(ConfigContainer configInfo);
	string getClassName(string originFileName);

};