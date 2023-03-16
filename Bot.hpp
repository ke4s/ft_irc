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

	Bot(char *dest_ip, int dest_port, char *dict_file) : sockATTR(), ircServATTR(), destIp(dest_ip), destPort(dest_port), dictionary(dict_file)
	{
		if ((sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) // IPV4 TCP özelliklerine sahip socket oluşturuldu.
			throw std::runtime_error(strerror(errno));

		sockATTR.sin_family = AF_INET; //IPV4
		sockATTR.sin_addr.s_addr = htons(INADDR_ANY);	//bu tanım bulunduğu bilgisayarın ip adresini almasını söyler //inet_addr("127.0.0.1"); // htonl(INADDR_ANY)  INADDR_ANY sabitinin mesela 12 olarak belirlendiği durumlarda işe yarar.
		sockATTR.sin_port = 0;							// İşletim sistemi boşta olan bir portu atayacak.
		memset(sockATTR.sin_zero, 0, 8);

		// I am trying to use connect().
		ircServATTR.sin_family = AF_INET;
		ircServATTR.sin_addr.s_addr = inet_addr(dest_ip);
		ircServATTR.sin_port = htons(destPort);
		memset(ircServATTR.sin_zero, 0, 8);

		if (connect(sockFD, reinterpret_cast<sockaddr *>(&ircServATTR), sizeof(ircServATTR)) <= 0)
			throw std::runtime_error(strerror(errno));
	}
	/*
	void binding(void)
	{
		if (bind(sockFD, reinterpret_cast<struct sockaddr *>(&sockATTR), sizeof(sockATTR)) < 0)
		{
			throw std::runtime_error(strerror(errno));
		}
	}

	void listenSock(int backlog)
	{
		if (!(backlog > 1 && backlog < 20))
			throw std::runtime_error(strerror(errno));

		if (listen(sockFD, backlog) < 0)
			throw std::runtime_error(strerror(errno));
	}

	void	acceptIrcServer(void)
	{
		socklen_t	sock_len = sizeof(ircServATTR);
		if ((ircServFD = accept(sockFD, reinterpret_cast<sockaddr*>(&ircServATTR), &size_client)) < 0)
			throw std::runtime_error(strerror(errno));

		close(sockFD); // I want to accept just one fd so I close  my socket.
	}
	*/

	void catchBadWords()
	{
		char *buff = (char *)malloc(512);
		string badWord;
		while (1)
		{
			if (recv(sockFD, buff, strlen(buff), 0))
				throw std::runtime_error(strerror(errno));
			if ((badWord = checkWordsInFile(dictionary, buff)).length() != 0)
			{
				string banMessage = buff + badWord;
				if (send(sockFD, buff, banMessage.length(), 0) < 0)
					throw std::runtime_error(strerror(errno));
			}
		}
	}

	std::string checkWordsInFile(string file_name, string sentence)
	{
		string word;
		ifstream file(file_name);
		if (file.is_open())
		{
			while (!file.eof())
			{
				getline(file,word);
				if (sentence.find(word, 0) != string::npos)
				{
					file.close();
					return word;
				}
			}
			file.close();
			return string("");
		}
		else
			throw std::runtime_error("Can't open ");
	}

};