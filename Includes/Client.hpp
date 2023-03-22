#pragma once


#include <string>
#include <map>
#include "Channel.hpp"

using namespace std;

#define STATE_PASS 1
#define STATE_NICK 2
#define STATE_USER 3
#define STATE_REGISTERED 4
#define STATE_JOIN 5

#define STATE_BOT  6

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
	string	_hostName;
	string	_serverName;
	string	_password;
	short int	_state;
	bool		_isRegistered;
	bool		_isBot;
	string		_messBuff;

	map<string, Channel*>	joinedChannels;


public:

	Client(const int fd, struct sockaddr_in attr, string pass) : _sockFD(fd), _sockATTR(attr), _password(pass)
	{
		_state = STATE_PASS;
		_isRegistered = 0;
		_nickName = "";
		_fullName = "";
		_password = "";
		_messBuff = "";
		_isBot = false;
	}

	const bool& isRegistered(void)
	{
		return _isRegistered;
	}

	const struct sockaddr_in& getATTR(void)
	{ return _sockATTR; }

	void clearMessBuff()
	{
		_messBuff.clear();
		_messBuff = "";
	}

	void appendMessBuff(const string& mess)
	{
		_messBuff.append(mess);
	}

	const string& getMessBuff(void)
	{
		return _messBuff;
	}
};