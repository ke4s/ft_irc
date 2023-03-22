#include "../../Includes/Utils.hpp"

vector<string> splitString(const string& inputStr)
{
	istringstream iss(inputStr);
	vector<string> words;

	string word;
	while (iss >> word)
		words.push_back(word);
	return words;
}

bool checkIsInFile(const string& file_name, const string& to_find)
{
	string line;
	ifstream file(file_name);

	if (!file)
	{
		(ofstream (file_name)).close();
		file.close();
		file.open(file_name);
	}

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

string randomStringGenerator(const int n)
{
	 string str = "";

	 srand(time(0));

	 for (int i = 0; i < n; ++i) {
		 str.append(1, "0123456789@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"[rand() % 69]);
	 }
	 return str;
 }