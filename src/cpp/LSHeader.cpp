#include "LSHeader.h"
#include "Utils.h"

string LSHeader::getHeader(ConfigContainer configInfo) {
	
	string returnBuffer;
	
	returnBuffer += "//Name: ";
	returnBuffer += configInfo.name;
	returnBuffer += "\n//Period: ";
	returnBuffer += configInfo.period;
	returnBuffer += "\n//Date: ";
	returnBuffer += configInfo.date == "" ? Utils::getSystemDate() : configInfo.date;
	returnBuffer += "\n//Project Title: ";
	returnBuffer += configInfo.pTitle == "" ? Utils::removeDotJava(configInfo.originFileName) : configInfo.pTitle;
	returnBuffer += "\n//Project Description: ";
	returnBuffer += configInfo.pDescription == "" ? ("A program for satisfying the deamands of the " + configInfo.pTitle + " Assignment.") : configInfo.pDescription;
	returnBuffer += "\n//Version: ";
	returnBuffer += configInfo.version == "" ? "0.0.1" : configInfo.version;

	return returnBuffer;
}

string LSHeader::getClassName(string originFileName) {
	
	//Init
	int startPosition = 0;
	int endPosition = originFileName.size() - 1;
	string returnBuffer;

	//Remove .java
	if (originFileName.find(".java")) endPosition -= 5;
	

	//Remove folders
	if (originFileName.find("/")) startPosition = originFileName.find_last_of("/") + 1;

	//Start generating output
	for (int n = startPosition; n <= endPosition; ++n) returnBuffer += originFileName[n];

	return returnBuffer;

}