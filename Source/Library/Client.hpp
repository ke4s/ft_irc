#pragma once


#include <string>
#include <map>
#include "Channel.hpp"

using namespace std;

#define STATE_PASS 1
#define STATE_NICK 2
#define STATE_USER 3
#define STATE_JOIN 4
#define STATE_BOT  5

#define MSG_LOGIN 12
#define MSG_EVENT 14
#define MSG_ALL	  13
#define MSG_PRIV  15

#define BOT_KEY   "8037cf0192cfc4a19e9c5ad482c8fa7ede6f8ba26aa8a091f53dc3e42b6d09a7"

class Client
{
public:
	int		_sockFD;
	struct sockaddr_in _sockATTR;

	string	_nickName;
	string	_fullName;
	string	_password;
	short int	_state;
	bool		_isRegistered;
	bool		_isBot;

	map<string, Channel*>	joinedChannels;


public:
	Client(const int fd, struct sockaddr_in attr, string pass) : _sockFD(fd), _sockATTR(attr), _password(pass)
	{
		_state = STATE_PASS;
		_isRegistered = 0;
		_isBot = false;
	}

	int RecieveMessage(string message)
	{
		if (_state == STATE_PASS)
		{
			isBOT(message);
			string command = message.substr(0, message.find_first_of(' '));
			if (command == "PASS")
			{
				string received_pass = message.substr(message.find_first_of(' ') + 1, message.length());
				if (received_pass.find(_password) != string::npos)
				{
					cout <<  "Somebody Registered" << endl;
					_state = STATE_USER;
					_isRegistered = 1;
				}
			}
			return MSG_LOGIN;
		}
		return MSG_ALL;
	}

	void isBOT(string message)
	{
		string word;
		if (message == BOT_KEY)
		{
			_isBot = 1;
			_state = STATE_BOT;
			_isRegistered = 1;
			cout << "BOT AKTİF" << endl;
		}
	}

	const bool& isRegistered(void)
	{
		return _isRegistered;
	}

	const struct sockaddr_in& getATTR(void)
	{ return _sockATTR; }

};