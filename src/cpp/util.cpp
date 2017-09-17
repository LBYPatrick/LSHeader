#pragma once
#include "LSHeader.h"
#include "util.h"

string util::toUpperString(string str) {
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		*it = (char)toupper(*it);
	return str;
}

bool util::containIgnoreCase(string str, string key, bool strict) {
	if (strict) return toUpperString(str) == toUpperString(key);
	if (!strict) return toUpperString(str).find(toUpperString(key)) != string::npos;
}

string util::getSystemDate() {
	
	string endResult;
	time_t now = time(0);
	tm*timeContainer = localtime(&now);
	
	switch (1 + timeContainer->tm_mon) {
	case 1: endResult += "January";		break;
	case 2: endResult += "February";	break;
	case 3: endResult += "March";		break;
	case 4: endResult += "April";		break;
	case 5: endResult += "May";			break;
	case 6: endResult += "June";		break;
	case 7: endResult += "July";		break;
	case 8: endResult += "August";		break;
	case 9: endResult += "September";	break;
	case 10:endResult += "October";		break;
	case 11:endResult += "November";	break;
	case 12:endResult += "December";	break;
	default: "Unknown";
	}

	endResult += " ";
	endResult += std::to_string(timeContainer->tm_mday);

	int bufDate = timeContainer->tm_mday;

	while (bufDate > 10) bufDate -= 10;

	switch (bufDate) {
	case 1: endResult += "st";break;
	case 2: endResult += "nd";break;
	case 3: endResult += "rd";break;
	default:endResult += "th";
	};

	endResult += ", ";
	endResult += std::to_string(1900 + timeContainer->tm_year);


	return endResult;
}