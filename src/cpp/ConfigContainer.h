#pragma once
#include<string>

using std::string;


struct ConfigContainer final {
	string name = "";
	string period = "";
	string date = "";
	string pTitle = "";
	string pDescription = "";
	string version = "";
	string originFileName;
	string inputFile;
	string outputFile;
};
