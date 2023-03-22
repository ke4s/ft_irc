#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string>
#include <stdlib.h>
#include <stdexcept>

#include <fstream>

#define BOT_KEY   "8037cf0192cfc4a19e9c5ad482c8fa7ede6f8ba26aa8a091f53dc3e42b6d09a7"


using namespace std;

class Bot
{

private:
	int		sockFD;
	struct	sockaddr_in sockATTR;

	int 	ircServFD;
	struct	sockaddr_in ircServATTR;

	string	destIp;
	int		destPort;

	string dictionary; //encounter the incoming data with the words in this file.
	
public:

	Bot(char *dest_ip, int dest_port, char *dict_file, char* server_password);

	void catchBadWords();

	std::string checkWordsInFile(string file_name, string sentence);

};