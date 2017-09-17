#pragma once
#include "LSHeader.h"

namespace util {
	string getSystemDate();
	string toUpperString(string str);
	bool containIgnoreCase(string str, string key, bool strict = 0);
};