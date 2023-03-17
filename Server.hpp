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

#include <fstream>

#include "Client.hpp"
using namespace std;


//şu an bot'a mesaj gönderemiyor. Bot'u program içinden de çalıştıramıyorum. Debug modunda inceleme gerek.


class Server
{
private:

	typedef std::vector<struct pollfd>::iterator pollfds_it;
	typedef std::map<int, Client*>::iterator clients_it;

	int					serverSockFD;
	string				password;
	struct sockaddr_in	server_attr;

	vector<struct pollfd>		pollfds;
	map<int, Client*>			clients;

	string banFile;

public:

	Server(const int port, const string password, char *ban_file_name) : server_attr()
	{
		banFile = ban_file_name;
		memset(server_attr.sin_zero, 0, 8);

		if ((serverSockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) // IPV4 TCP özelliklerine sahip socket oluşturuldu.
			std::cout << "throw socket" << std::endl;

		//setsocketopt() kullanmamız gerekebilir.

		if (fcntl(serverSockFD, F_SETFL, O_NONBLOCK) == -1)
			throw std::runtime_error("Error while setting socket to NON-BLOCKING.");

		server_attr.sin_family = AF_INET; //IPV4
		server_attr.sin_addr.s_addr = htons(INADDR_ANY);//bu tanım bulunduğu bilgisayarın ip adresini almasını söyler //inet_addr("127.0.0.1"); // htonl(INADDR_ANY)  INADDR_ANY sabitinin mesela 12 olarak belirlendiği durumlarda işe yarar.
		std::cout << port << password << "\n";
		server_attr.sin_port = htons(port);
		this->password = password;

		pollfds.push_back(newPollfd(serverSockFD, POLL_IN, 0));
	}

	void binding(void)
	{
		if (bind(serverSockFD, reinterpret_cast<struct sockaddr *>(&server_attr), sizeof(server_attr)) < 0)
		{
			throw std::runtime_error("ERROR binding");
		}
	}

	void listenSock(int backlog)
	{
		if (!(backlog > 1 && backlog < 20))
			throw std::runtime_error("backlock must be in 2 beetwen 20");

		if (listen(serverSockFD, backlog) < 0)
			throw std::runtime_error("Listening error");
		//char *pwd = getcwd(NULL, 0);

		//execl(::strcat(getcwd(NULL, 0), "Bot"), "127.0.0.1", "6667", "badWords.txt");
	}

	void start()
	{
		//server is ready.
		cout << "Server is ready" << endl;
		while (1) // running değişkeni kullan
		{

			if (poll(pollfds.begin().base(), pollfds.size(), 10) < 0)
				throw	std::runtime_error("Error while polling from fd.\n");
			
			for (pollfds_it it = pollfds.begin(); it != pollfds.end(); it++)
			{
				if (it->revents == 0)
					continue;

				if ((it->revents & POLLHUP) == POLLHUP)
				{
					clientDisconnect(it->fd);
					break;
				}
				if (it->revents == POLL_IN)
				{
					if (it->fd == serverSockFD)
						clientConnect();
					else
						clientSendingMSG(it->fd);
					break;
				}
			}
		}

	}

	void	clientConnect()
	{
		struct sockaddr_in	newClientATTR;
		int					newClientFD;
		socklen_t           size_client  = sizeof(newClientATTR);

		memset(&newClientATTR, 0, sizeof(struct sockaddr_in));

		if ((newClientFD = accept(serverSockFD, reinterpret_cast<sockaddr*>(&newClientATTR), &size_client)) < 0)
			throw std::runtime_error("Error while accepting new client.");

		if (checkIsBanned(newClientATTR))
		{
			cout << inet_ntoa(newClientATTR.sin_addr) << ":" << ntohs(newClientATTR.sin_port) <<" Banned User trying to connect" << endl;
			send(newClientFD, "You are BANNED. You can't join the server.\n", 44, 0);
			close(newClientFD);
		}
		else
		{
			pollfds.push_back(newPollfd(newClientFD, POLL_IN, 0));
			clients.insert(make_pair(newClientFD, new Client(newClientFD, newClientATTR)));
			cout << inet_ntoa(newClientATTR.sin_addr) << ":" << ntohs(newClientATTR.sin_port) << " Client connected to server" << endl;
		}
	}

	void clientDisconnect(int fd)
	{
		clients_it it_client = clients.find(fd);

		cout << inet_ntoa(it_client->second->getATTR().sin_addr) << ":" << ntohs(it_client->second->getATTR().sin_port) << " Client Disconnected" << endl;

		delete it_client->second;
		clients.erase(it_client);

		for (pollfds_it it_poll = pollfds.begin(); it_poll != pollfds.end(); it_poll++)
		{
			if (it_poll->fd == fd)
			{
				pollfds.erase(it_poll);
				break;
			}
		}
		close(fd);
	}

	void clientSendingMSG(int fd)
	{
		char *buff = (char *)malloc(512);
		if (recv(fd, buff, 512, 0) < 0)
			throw std::runtime_error("Recv error");

		string to_send = buff;
		if (clients.find(fd)->second->RecieveMessage(buff) == MSG_ALL)
		{
			for (pollfds_it it = ++(pollfds.begin()); it < pollfds.end(); ++it) {
				if (it->fd != fd)
					send(it->fd, buff, ::strlen(buff), 0);
			}
		}
	}

	const int& getFd() { return serverSockFD; }
	const string& getPasw() { return password; }

private:

	bool checkIsInFile(string file_name, string to_find)
	{
		string line;
		ifstream file(file_name);
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

	bool checkIsBanned(struct sockaddr_in Client)
	{
		string ip_to_find = inet_ntoa(Client.sin_addr);

		if (checkIsInFile(banFile, ip_to_find))
			return true;
		return false;
	}

	struct pollfd newPollfd(int sock_fd, int events, int revents)
	{
		struct pollfd ret;
		ret.fd = sock_fd;
		ret.events = events;
		ret.revents = revents;
		return ret;
	}

};