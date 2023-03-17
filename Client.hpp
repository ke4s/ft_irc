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
	int		sockFD;
	struct sockaddr_in sockATTR;

	string	nickName;
	string	fullName;
	string	password;
	short int	state;
	bool		isRegistered;
	bool		isBot;

	map<string, Channel*>	joinedChannels;


public:
	Client(const int fd, struct sockaddr_in attr) : sockFD(fd), sockATTR(attr)
	{
		state = STATE_PASS;
		isRegistered = false;
		isBot = false;
	}

	int RecieveMessage(string message)
	{
		if (state == STATE_PASS)
		{
			GetPASS(message);
			return MSG_LOGIN;
		}
		return MSG_ALL;
	}

	void GetPASS(string message)
	{
		if (message == BOT_KEY)
		{
			isBot = 1;
			state = STATE_BOT;
			cout << "BOT GELDİ CNM" << endl;
		}
		else if (message.find("PASS") == 0)
		{
			state = message.find(password, 5) == 5 ? STATE_JOIN : STATE_PASS;
			cout << message << state << endl;
		}
	}


	const struct sockaddr_in& getATTR(void)
	{ return sockATTR; }

};