#pragma once

#include "Common.h"
#include <set>

using namespace std;

class InputParser
{
private:
	vector<string> input;
	//map<string, int> int_args;
	//set<string> bool_args;
public:
	InputParser(int argc, char** argv);
	bool flag_exists(string flag);		    //check whether a single flag exists
	bool get_string(string flag, string& value); //return string value after the flag
	bool get_int(string flag, int& value); //return int value after the flag
	bool get_double(string flag, double& value);

	//void print_args();

	//bool get_int(const string flag, int& value);
	//bool bool_exists(const string value);
	

	//map<string, int>& get_int_args();
	//set<string>& get_bool_args();
};
