#pragma once

#include "vector"
#include "string"
#include "fstream"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

vector<string> splitString(const string& inputStr);

bool checkIsInFile(const string& file_name, const string& to_find);

string randomStringGenerator(const int n);
