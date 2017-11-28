#pragma once
#include<iostream>
#include<fstream>
#include<ctime>
#include<string>
#include "Utils.h"
#include "ConfigContainer.h"

using std::ifstream;
using std::ofstream;
using std::string;
using namespace Utils;

ifstream reader;
ofstream writer;

string readBuffer;
string inputFile = "";
string outputFile = "";
string failReasons;

ConfigContainer config;

bool checkFileAccess(string i, string o) {
	
	reader.open(i.c_str());
	writer.open(o.c_str());

	if (!reader.is_open()) { failReasons += "\nProgram cannot find the origin file."; return 1; }
	if (!writer.is_open()) { failReasons += "\nProgram cannot output to the target file."; return 1; }
	
	reader.close();
	writer.close();
	return 0;

}



bool readConfig() {
	
	reader.open("lsheader.ini");
	
	if (!reader.is_open()) {
		//printf(R"(Config "lsheader.ini" cannot be read, please check file access and make sure it exists.)");
		return 1;
	}
	
	while (getline(reader, readBuffer)) {
		
		if (containIgnoreCase(readBuffer, "name")) {
			
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				config.name += readBuffer[startPos];
			}

		}		//Filter for Name

		if (containIgnoreCase(readBuffer, "date")) {
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				config.date += readBuffer[startPos];
			}
		}		//Filter for Date

		if (containIgnoreCase(readBuffer, "period")) {
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				config.period += readBuffer[startPos];
			}

		}		//Filter for Period

		if (containIgnoreCase(readBuffer, "title")) {

			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				config.pTitle += readBuffer[startPos];
			} 

		} //Filter for Project Title

		if (containIgnoreCase(readBuffer, "descri")) {
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				config.pDescription += readBuffer[startPos];
			}
		} //Filter for Project Description
		if (containIgnoreCase(readBuffer, "version")) {
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				config.version += readBuffer[startPos];
			}
		}
	}
	reader.close();
	return 0;
}

bool checkMissingParameters() {
	bool returnVal = 0;
	if (config.name == "")					{ failReasons += "Need to specify name.\n"; returnVal = 1;}
	else if (config.period == "")				{ failReasons += "Need to specify period.\n"; returnVal = 1;}
	else if (inputFile == "")			{ failReasons += "Need to specify input file.\n";returnVal = 1;}


	return returnVal;
}

int main(int argc, char*const argv[]) {
	
	string * documentCacheSpace = new string();
	//string outputClassName;
	

	readConfig(); // Read the config before running

	for (int i = 1; i < argc; ++i) {
		if (containIgnoreCase(argv[i], "-i", 1) || containIgnoreCase(argv[i], "--input")) { inputFile = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-o", 1) || containIgnoreCase(argv[i], "--output")) { outputFile = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-a", 1) || containIgnoreCase(argv[i], "--author")) { config.name = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-d", 1) || containIgnoreCase(argv[i], "--date")) { config.date = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-p", 1) || containIgnoreCase(argv[i], "--period")) { config.period = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-pt", 1) || containIgnoreCase(argv[i], "--project-title")) { config.pTitle = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-pd", 1) || containIgnoreCase(argv[i], "--project-description")) { config.pDescription = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-v", 1) || containIgnoreCase(argv[i], "--version")) { config.version = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-h", 1) || containIgnoreCase(argv[i], "");
	}
	//=================================
	//Check missing parameters
	//=================================
	if (checkMissingParameters())					 { printf("%s\nFailed to execute.\n", failReasons.c_str()); return 1; } 
	
	

	//====================
	//Check File Access
	//====================
	if (checkFileAccess(inputFile, outputFile)) { printf("%s\nFailed to execute.\n", failReasons.c_str()); return 1; }



	printf("success.");

	return 0;
}