#include "../../Includes/Server.hpp"

clients_it Server::findClientsByNick(const string& nick)
{
	clients_it it = clients.begin();
	while (it != clients.end())
	{
		if (it->second->_nickName == nick)
			return it;
		it++;
	}
	return it;
}

bool Server::checkIsBanned(struct sockaddr_in Client)
{
	string ip_to_find = inet_ntoa(Client.sin_addr);

	if (checkIsInFile(banFile, ip_to_find))
		return true;
	return false;
}

struct pollfd Server::newPollfd(int sock_fd, int events, int revents)
{
	struct pollfd ret;
	ret.fd = sock_fd;
	ret.events = events;
	ret.revents = revents;
	return ret;
}