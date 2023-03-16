#pragma once


#include <string>
#include <map>
#include "Channel.hpp"
using namespace std;
#define STATE_PASS 1
#define STATE_NICK 2
#define STATE_USER 3
#define STATE_JOIN 3

class Client
{
private:
	int		sockFD;
	struct sockaddr_in sockATTR;

	string	nickName;
	string	fullName;

	short int state;
	bool			isRegistered;

	map<string, Channel*>	joinedChannels;


public:
	Client(const int fd, struct sockaddr_in attr) : sockFD(fd), sockATTR(attr)
	{
		state = STATE_PASS;
		isRegistered = 0;
	}

	const struct sockaddr_in& getATTR(void)
	{ return sockATTR; }

};