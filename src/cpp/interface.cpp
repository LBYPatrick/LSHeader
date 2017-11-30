#pragma once
#include<iostream>
#include<fstream>
#include<ctime>
#include<string>
#include "Utils.h"
#include "ConfigContainer.h"
#include "LSHeader.h"

using std::ifstream;
using std::ofstream;
using std::string;
using namespace Utils;

ifstream reader;
ofstream writer;

bool checkFileAccess(string i, string o,string&failMessageBuffer) {
	
	reader.open(i.c_str());
	if (!reader.is_open()) { failMessageBuffer += "\nProgram cannot find the origin file."; reader.close(); return 1; }
	reader.close();


	writer.open(o.c_str(), std::ofstream::app);
	if (!writer.is_open()) { failMessageBuffer += "\nProgram cannot output to the target file.";  writer.close(); return 1;}
	writer.close();
	
	return 0;

}



bool readConfig(ConfigContainer&config) {
	
	string readBuffer;
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

bool checkMissingParameters(ConfigContainer&config, string & failMessageBuffer) {
	bool returnVal = 0;
	if (config.name == "")					{ failMessageBuffer += "Need to specify name.\n"; returnVal = 1;}
	else if (config.period == "")				{ failMessageBuffer += "Need to specify period.\n"; returnVal = 1;}
	else if (config.inputFile == "")			{ failMessageBuffer += "Need to specify input file.\n";returnVal = 1;}


	return returnVal;
}

int main(int argc, char*const argv[]) {
	

	bool			headerOnly		   = false;
	string  		rawFileContent;
	ConfigContainer config;
	string			failReasons;
	string			readBuffer;
	string			rawClassName;
	string			finalClassName;
	string			lineBuffer;
	bool			isEntryClassScanned = false;
	

	readConfig(config); // Read the config before running

	for (int i = 1; i < argc; ++i) {

		if (checkParam(argv[i], "-i", "--input")) { config.inputFile = argv[i + 1]; ++i;}
		else if (checkParam(argv[i], "-o", "--output")) { config.outputFile = argv[i + 1]; ++i;}
		else if (checkParam(argv[i], "-a", "--author")) { config.name = argv[i + 1]; ++i;}
		else if (checkParam(argv[i], "-d", "--date")) { config.date = argv[i + 1]; ++i; }
		else if (checkParam(argv[i], "-p", "--period")) { config.period = argv[i + 1]; ++i; }
		else if (checkParam(argv[i], "-pt", "--project-title")) { config.pTitle = argv[i + 1]; ++i; }
		else if (checkParam(argv[i], "-pd", "--project-description")) { config.pDescription = argv[i + 1]; ++i; }
		else if (checkParam(argv[i], "-v", "--version")) { config.version = argv[i + 1]; ++i; }
		else if (checkParam(argv[i], "-H", "--header-only")) { headerOnly = true;};
	}
	//Check missing parameters
	if (checkMissingParameters(config,failReasons))					 { printf("%s\nFailed to execute.\n", failReasons.c_str()); return 1; } 
	
	//Format outputFile name
	if (config.outputFile == "") {
		for (int i = 0; i <= config.inputFile.find_last_of(".java") - 5; ++i) { rawClassName += config.inputFile[i]; }
		
		finalClassName = rawClassName;

		if (!headerOnly) {
			for (char buffer : config.name) { if (buffer != ' ') finalClassName += toupper(buffer); } 
			config.outputFile = finalClassName + ".java";
		}
		else config.outputFile = config.inputFile;
	}

	//Generate Project Title if not found
	config.pTitle = config.pTitle == "" ? rawClassName : config.pTitle;

	//Check File Access
	if (checkFileAccess(config.inputFile, config.outputFile,failReasons)) { printf("%s\nFailed to execute.\n", failReasons.c_str()); return 1; }

	//Read inputFile Content
	reader.open(config.inputFile.c_str());

	while (getline(reader, readBuffer)) {
		
		if (!isEntryClassScanned) { // Replace class name
			if (readBuffer.find("public class " + rawClassName) != string::npos) {
				for (int i = 0; i < readBuffer.find(rawClassName); ++i) lineBuffer += readBuffer[i];
				lineBuffer += finalClassName;
				for (int i = (rawClassName.find(rawClassName)) + rawClassName.size(); i < rawClassName.size(); ++i)
					lineBuffer += readBuffer[i];

				rawFileContent += lineBuffer;
				rawFileContent += "\n";
				isEntryClassScanned = true;
				continue;
			}
		}

		rawFileContent += readBuffer;
		rawFileContent += "\n";
	}
	reader.close();

	//Write
	writer.open(config.outputFile.c_str());
	writer << LSHeader::getHeader(config)
		<< "\n\n\n"
		<< rawFileContent
		<< "\n";
	writer.close();

	printf("success.");

	return 0;
}