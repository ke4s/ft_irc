#include "../../Includes/Server.hpp"

void Server::Privmsg(const vector<string>& splited_message, string message, Client *recvClient)
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
		clientMSG(splited_message, message, recvClient);
}