#pragma once

#include "vector"
#include "string"
#include "fstream"
#include <iostream>
#include <sstream>

using namespace std;

inline vector<string> splitString(const string& inputStr)
{
    istringstream iss(inputStr);
    vector<string> words;

    string word;
    while (iss >> word)
        words.push_back(word);
	return words;
}

inline bool checkIsInFile(string file_name, string to_find)
{
	string line;
	ifstream file(file_name);
	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file,line);
			if (line.find(to_find, 0) != string::npos)
			{
				file.close();
				return true;
			}
		}
		file.close();
		return false;
	}
	else
		throw std::runtime_error("Can't open ");
}