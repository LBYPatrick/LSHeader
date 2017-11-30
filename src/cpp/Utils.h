#pragma once
#include<iostream>
#include<fstream>
#include<ctime>
#include<string>

using std::ifstream;
using std::ofstream;
using std::string;


namespace Utils {
	string getSystemDate();
	string toUpperString(string str);
	bool containIgnoreCase(string str, string key, bool strict = 0);
	string removeDotJava(string str);
	bool checkParam(string input,string abbr, string full);
};