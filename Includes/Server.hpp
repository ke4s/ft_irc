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


#include "Utils.hpp"
#include "IRC_COMMAND.hpp"

using namespace std;


//şu an bot'a mesaj gönderemiyor. BotResources'u program içinden de çalıştıramıyorum. Debug modunda inceleme gerek.


typedef std::vector<struct pollfd>::iterator pollfds_it;
typedef std::map<int, Client*>::iterator clients_it;

class Server
{
private:
	int					serverSockFD;
	string				password;
	struct sockaddr_in	server_attr;

	int	bot_fd;

	vector<struct pollfd>		pollfds;
	map<int, Client*>			clients;

	//map<channel name, pair<admin Client*, channel topic> > if channels has another attributes we have to change pair to vector or channel class or something.
	map<string, pair<Client*, string> >	channels;

	//multimap<channel name, member Client's pointer>
	multimap<string, Client*>	channel_members;

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

	void sendHUB(string message, Client *recvClient);
	void clientLOGIN(const vector<string>& splited_message, Client *recvClient);
	void clientEVENTS(vector<string> splited_message, Client* recvClient);
	void clientMSG(vector<string> splited_message, Client* recvClient);




	int isBot(string message,Client *recvClient)
	{
		string word;
		if (message == BOT_KEY)
		{
			recvClient->_isBot = 1;
			recvClient->_state = STATE_BOT;
			recvClient->_isRegistered = 1;
			cout << "BOT Activated" << endl;
			bot_fd = recvClient->_sockFD;
			return 1;
		}
		return 0;
	}

	clients_it findClientsByNick(const string& nick);
	bool checkIsBanned(struct sockaddr_in Client);
	struct pollfd newPollfd(int sock_fd, int events, int revents);

};