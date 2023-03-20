#include "../../Includes/Server.hpp"

int Server::sendHUB(string message, Client *recvClient)
{
	if (isBot(message, recvClient))
		return MSG_LOGIN;

	vector<string> splited_message = splitString(message);

	if (splited_message.empty())
		return -1;

	if (splited_message[0] == "PASS" || splited_message[0] == "NICK") // command find
	{
		return clientLOGIN(splited_message, recvClient);
	}
	else if (splited_message[0] == "PRIVMSG" || recvClient->_state == STATE_BOT)
	{
		for (pollfds_it it = ++(pollfds.begin()); it < pollfds.end(); ++it)
		{
			if (it->fd != recvClient->_sockFD && clients.find(it->fd)->second->isRegistered())
				send(it->fd, message.c_str(), message.length(), 0);
		}
	}
	else if (splited_message[0] == "QUIT" || splited_message[0] == "KICK" || splited_message[0] == "PART") // quit Güle güle diyip client'ı fd'sini kapatır.
	{
		return MSG_EVENT;
	}
	else
	{
		string to_send = ERR_UNKNOWNCOMMAND(recvClient->_nickName, splited_message[0]);
		send(recvClient->_sockFD, to_send.c_str(), to_send.length(), 0);
		return 1;
	}
	return -1;
}

void Server::clientSendingMSG(int fd)
{
	char *buff = (char *)malloc(512);

	if (recv(fd, buff, 512, 0) < 0) //Recv fonksiyonunu geliştirebiliriz.
		throw std::runtime_error("Recv error");

	string to_send = buff;

	Client *recvClient = clients.find(fd)->second;

	cout << inet_ntoa(recvClient->_sockATTR.sin_addr) << ":" << ntohs(recvClient->_sockATTR.sin_port) << ":" << recvClient->_nickName << ": " << buff;

	sendHUB(to_send, recvClient);
}

int Server::clientLOGIN(const vector<string>& splited_message, Client *recvClient)
{
	if (splited_message[0] == "PASS")
	{
		if (recvClient->_state == STATE_PASS)
		{
			string recv_password = splited_message[1];
			if (recv_password == password)
			{
				recvClient->_state = STATE_NICK;
				cout <<  "Somebody Registered" << endl;
				recvClient->_isRegistered = 1; //test amaclı
			}
		}
		else
		{
			send(recvClient->_sockFD, "You can't PASS", 15, 0);
		}
		return MSG_LOGIN;
	}
	else if (splited_message[0] == "NICK")
	{
		if (recvClient->_state == STATE_NICK)
		{
			if (findClientsByNick(splited_message[1]) != clients.end())
			{
				string send_to = ERR_NICKNAMEINUSE(splited_message[1]);
				send(recvClient->_sockFD, send_to.c_str(), send_to.length(), 0);
			}
			else
			{
				recvClient->_nickName = splited_message[1];
				recvClient->_state = STATE_USER;
			}
		}
		else
		{
			send(recvClient->_sockFD, "You can't NICK", 15, 0);
		}
	}
	return MSG_ALL;
}
