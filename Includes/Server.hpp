#pragma once

#include <vector>
#include <map>

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string>
#include <stdlib.h>
#include <stdexcept>
#include <signal.h>

#include <fstream>

#include "Client.hpp"
#include "Channel.hpp"

#include "Commands.hpp"
#include "CommandCenter.hpp"
#include "Utils.hpp"
#include "IRC_COMMAND.hpp"

class CommandCenter;
using namespace std;


//şu an bot'a mesaj gönderemiyor. BotResources'u program içinden de çalıştıramıyorum. Debug modunda inceleme gerek.


typedef std::vector<struct pollfd>::iterator pollfds_it;
typedef std::map<int, Client*>::iterator clients_it;


class Server
{
private:

	CommandCenter *commandcenter;

	int					serverSockFD;
	string				password;
	struct sockaddr_in	server_attr;

	vector<struct pollfd>		pollfds;
	map<int, Client*>			clients;

	string banFile;

public:

	Server(const int port, const string password, char *ban_file_name);

	void binding(void);
	void listenSock(int backlog);

	void acceptingRequest();


	const int& getFd() { return serverSockFD; }
	const string& getPasw() { return password; }

private:

	void clientConnect();
	void clientDisconnect(int fd);



	void clientSendingMSG(int fd);

	int sendHUB(string message, Client *recvClient);
	int clientLOGIN(const vector<string>& splited_message, Client *recvClient);


private:

	int isBot(string message,Client *recvClient)
	{
		string word;
		if (message == BOT_KEY)
		{
			recvClient->_isBot = 1;
			recvClient->_state = STATE_BOT;
			recvClient->_isRegistered = 1;
			cout << "BOT Activated" << endl;
			return 1;
		}
		return 0;
	}

	clients_it findClientsByNick(const string& nick);
	bool checkIsBanned(struct sockaddr_in Client);
	struct pollfd newPollfd(int sock_fd, int events, int revents);

};