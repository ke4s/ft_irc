#include "../../Includes/Server.hpp"

void	Server::clientConnect()
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
		clients.insert(make_pair(newClientFD, new Client(newClientFD, newClientATTR, password)));
		cout << inet_ntoa(newClientATTR.sin_addr) << ":" << ntohs(newClientATTR.sin_port) << " Client connected to server" << endl;
	}
}

void Server::clientDisconnect(int fd)
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