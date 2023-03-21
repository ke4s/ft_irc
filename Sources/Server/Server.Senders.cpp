#include "../../Includes/Server.hpp"

void Server::sendHUB(string message, Client *recvClient)
{

	if (isBot(message, recvClient))
		return ;

	vector<string> splited_message = splitString(message);


	if (splited_message.empty())
		return ;

	if (bot_fd && !recvClient->_isBot)
		send(bot_fd, message.c_str(), message.length(), 0);

	string command = splited_message[0];

	if (command == "PASS" || command == "NICK" || command == "USER")
	{
		clientLOGIN(splited_message, recvClient);
	}
	else if (command == "PRIVMSG" || recvClient->_state == STATE_BOT)
	{
		if (recvClient->_state == STATE_BOT)
		{
			for (pollfds_it it = ++(pollfds.begin()); it < pollfds.end(); ++it)
			{
				if (it->fd != recvClient->_sockFD && clients.find(it->fd)->second->isRegistered())
					send(it->fd, message.c_str(), message.length(), 0);
			}
		}
		else
			clientMSG(splited_message, recvClient);
	}
	else if (command == "QUIT" || command == "KICK" || command == "PART" || command == "JOIN") // quit Güle güle diyip client'ı fd'sini kapatır.
	{
		clientEVENTS(splited_message, recvClient);
	}
	else
	{
		string to_send = ERR_UNKNOWNCOMMAND(recvClient->_nickName, command);
		send(recvClient->_sockFD, to_send.c_str(), to_send.length(), 0);
	}
	return ;
}

void Server::clientSendingMSG(int fd)
{
	char *buff = (char *)malloc(512);

	if (recv(fd, buff, 512, 0) < 0) //Recv fonksiyonunu geliştirebiliriz.
		throw std::runtime_error("Recv error");

	string to_send = buff;
	free(buff);
	Client *recvClient = clients.find(fd)->second;

	cout << inet_ntoa(recvClient->_sockATTR.sin_addr) << ":" << ntohs(recvClient->_sockATTR.sin_port) << ":" << recvClient->_nickName << ":" << buff;

	sendHUB(to_send, recvClient);
}

