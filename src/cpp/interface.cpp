#include "LSHeader.h"
#include "util.h"

using namespace util;

ifstream reader;
ofstream writer;

string readBuffer;

string name = "";
string date = "";
string period = "";
string projectTitle = "";
string projectDescription = "";
string version = "";
string inputFile = "";
string outputFile = "";
string failReasons;

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
				name += readBuffer[startPos];
			}

		}		//Filter for Name

		if (containIgnoreCase(readBuffer, "date")) {
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				date += readBuffer[startPos];
			}
		}		//Filter for Date

		if (containIgnoreCase(readBuffer, "period")) {
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				period += readBuffer[startPos];
			}

		}		//Filter for Period

		if (containIgnoreCase(readBuffer, "title")) {

			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				projectTitle += readBuffer[startPos];
			} 

		} //Filter for Project Title

		if (containIgnoreCase(readBuffer, "descri")) {
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				projectDescription += readBuffer[startPos];
			}
		} //Filter for Project Description
		if (containIgnoreCase(readBuffer, "version")) {
			for (int startPos = readBuffer.find_first_of("=") + 1;
				startPos < readBuffer.size();
				++startPos) {
				version += readBuffer[startPos];
			}
		}
	}
	reader.close();
	return 0;
}

bool checkMissingParameters() {
	bool returnVal = 0;
	if (name == "")					{ failReasons += "Need to specify name.\n"; returnVal = 1;}
	else if (period == "")				{ failReasons += "Need to specify period.\n"; returnVal = 1;}
	else if (projectTitle == "")			{ failReasons += "Need to specify project title.\n"; returnVal = 1; }
	else if (projectDescription == "")	{ failReasons += "Need to specify project description.\n";returnVal = 1;}
	else if (inputFile == "")			{ failReasons += "Need to specify input file.\n";returnVal = 1;}


	return returnVal;
}

int main(int argc, char*const argv[]) {
	
	string * documentCacheSpace = new string();
	string originalClassName;
	string outputClassName;
	

	readConfig(); // Read the config before running

	for (int i = 1; i < argc; ++i) {
		if (containIgnoreCase(argv[i], "-i", 1) || containIgnoreCase(argv[i], "--input")) { inputFile = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-o", 1) || containIgnoreCase(argv[i], "--output")) { outputFile = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-a", 1) || containIgnoreCase(argv[i], "--author")) { name = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-d", 1) || containIgnoreCase(argv[i], "--date")) { date = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-p", 1) || containIgnoreCase(argv[i], "--period")) { period = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-pt", 1) || containIgnoreCase(argv[i], "--project-title")) { projectTitle = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-pd", 1) || containIgnoreCase(argv[i], "--project-description")) { projectDescription = argv[i + 1]; ++i; }
		else if (containIgnoreCase(argv[i], "-v", 1) || containIgnoreCase(argv[i], "--version")) { version = argv[i + 1]; ++i; }
	}
	//=================================
	//Check missing parameters
	//=================================
	if (checkMissingParameters())					 { printf("%s\nFailed to execute.\n", failReasons.c_str()); return 1; } 
	
	//=================================
	//Process class name and file name
	//=================================
	if (outputFile == "") { // Make a neatly-formatted name for Mr. Patel's assignments
		for (int i = 0; i < inputFile.find_last_of("."); ++i) {outputClassName += inputFile[i];}
		
		for (char nameBuffer : name) {
			if (nameBuffer == ' ') continue;
			else outputClassName += toupper(nameBuffer);
		}

		outputFile = outputClassName + ".java";
	}
	else {
		for (int i = 0; i < outputFile.find_last_of("."); ++i) {outputClassName += outputFile[i];}
	}

	//Find class name from the input file name
	for (int i = 0; i < inputFile.find_last_of("."); ++i) {originalClassName += inputFile[i];}


	//====================
	//Check File Access
	//====================
	if (checkFileAccess(inputFile, outputFile)) { printf("%s\nFailed to execute.\n", failReasons.c_str()); return 1; }


	// Main Execution START
	if (version == "") version = "0.0.1";
	if (date == "") date = getSystemDate();

	reader.open(inputFile);


	// =====================
	// Cache File content
	// =====================
	while (getline(reader, readBuffer)) {
		if (readBuffer.find(("class "+ originalClassName)) != string::npos) {
			
			*documentCacheSpace += "public class " + outputClassName;
			if (readBuffer.find('{') != string::npos) *documentCacheSpace += " {";
			*documentCacheSpace += "\n";

			continue;
		}
		*documentCacheSpace += readBuffer + "\n";
	}

	reader.close(); //Close Read stream

	writer.open(outputFile);

	//====================
	//Parse Header
	//====================

	writer << R"(//Name: )" << name << "\n"
		<< R"(//Date: )" << date << "\n"
		<< R"(//Period: )" << period << "\n"
		<< R"(//Project Title: )" << projectTitle << "\n"
		<< R"(//Project Description: )" << projectDescription << "\n"
		<< R"(//Version: )" << version << "\n\n\n"; 
	
	writer << *documentCacheSpace;
	delete documentCacheSpace;
	writer.close(); //Close Write stream
	

	//Main Execution End
	
	printf("success.");

	return 0;
}