#include "InputParser.h"

InputParser::InputParser(int argc, char** argv)
{
	uint64 i;

	if (argc < 2)
	{
		cout << "InputParser: no input arguments!\n";
		exit(1);
	}

	for (i = 1; i < argc; ++i)
		input.push_back(argv[i]);
/*
	for (i = 1; i < argc - 1; ++i)
	{
		if (is_int(argv[i+1]))
		{
			int_args[argv[i]] = atoi(argv[i+1]);
			++i;
		}
		else
			bool_args.insert(argv[i]);
	}
	if (i == argc - 1)
		bool_args.insert(argv[i]);
*/
}
/*
void InputParser::print_args()
{
	cout << "Integer args\n";
	for (auto it = int_args.begin(); it != int_args.end(); ++it)
	{
		cout << it->first << ": " << it->second << endl;
	}

	cout << "\nBool args\n";
	for (auto it = bool_args.begin(); it != bool_args.end(); ++it)
		cout << *it << endl;
}

map<string, int>& InputParser::get_int_args()
{
	return int_args;
}

set<string>& InputParser::get_bool_args()
{
	return bool_args;
}

bool InputParser::get_int(const string flag, int& value)
{
	map<string, int>::iterator it;

	it = int_args.find(flag);

	if (it != int_args.end())
	{
		value = it->second;
		return true;
	}
	return false;
}

bool InputParser::bool_exists(const string value)
{
	if (bool_args.find(value) != bool_args.end())
		return true;
	return false;
}
*/

bool InputParser::flag_exists(string flag)
{
	uint64 i;

	for (i = 0; i < input.size(); ++i)
		if (input.at(i) == flag)
			return true;
	return false;
}

bool InputParser::get_string(string flag, string& value)
{
	uint64 i;

	if (input.size() == 1)
		return false;

	for (i = 0; i < input.size() - 1; ++i)
		if (input.at(i) == flag)
		{
			value = input.at(i + 1);
			return true;
		}
	return false;
}


bool InputParser::get_int(string flag, int& value)
{
	uint64 i;

	if (input.size() == 1)
		return false;

	for (i = 0; i < input.size() - 1; ++i)
		if ((input.at(i) == flag) && (is_int(input.at(i+1).c_str())))
		{
			value = atoi(input.at(i + 1).c_str());
			return true;
		}
	return false;
}

bool InputParser::get_double(string flag, double& value)
{
	uint64 i;

	if (input.size() == 1)
		return false;

	for (i = 0; i < input.size() - 1; ++i)
		if ((input.at(i) == flag) && (is_double(input.at(i+1).c_str())))
		{
			value = atof(input.at(i + 1).c_str());
			return true;
		}
	return false;
}
