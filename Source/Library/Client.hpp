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
#define MSG_ALL	  13

#define BOT_KEY   "8037cf0192cfc4a19e9c5ad482c8fa7ede6f8ba26aa8a091f53dc3e42b6d09a7"

class Client
{
private:
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
	Client(const int fd, struct sockaddr_in attr) : _sockFD(fd), _sockATTR(attr)
	{
		_state = STATE_PASS;
		_isRegistered = false;
		_isBot = false;
	}

	int RecieveMessage(string message)
	{
		if (_state == STATE_PASS)
		{
			isBOT(message);
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
			cout << "BOT GELDİ CNM" << endl;
		}
	}

	const bool& isRegistered(void)
	{
		return _isRegistered;
	}

	const struct sockaddr_in& getATTR(void)
	{ return _sockATTR; }

};