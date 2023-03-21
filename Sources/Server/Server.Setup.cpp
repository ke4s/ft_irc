#include "../../Includes/Server.hpp"

Server::Server(const int port, const string password, char *ban_file_name) : server_attr(), bot_fd(0)
{
	banFile = ban_file_name;
	memset(server_attr.sin_zero, 0, 8);

	if ((serverSockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) // IPV4 TCP özelliklerine sahip socket oluşturuldu.
		std::cout << "throw socket" << std::endl;

	//setsocketopt() kullanmamız gerekebilir.

	if (fcntl(serverSockFD, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");

	server_attr.sin_family = AF_INET; //IPV4
	server_attr.sin_addr.s_addr = inet_addr("10.11.33.3"); //inet_addr("10.11.41.5");//bu tanım bulunduğu bilgisayarın ip adresini almasını söyler //inet_addr("127.0.0.1"); // htonl(INADDR_ANY)  INADDR_ANY sabitinin mesela 12 olarak belirlendiği durumlarda işe yarar.
	std::cout << port << password << "\n";
	server_attr.sin_port = htons(port);
	this->password = password;

	pollfds.push_back(newPollfd(serverSockFD, POLL_IN, 0));
}

//   Server::

void Server::binding(void)
{
	if (bind(serverSockFD, reinterpret_cast<struct sockaddr *>(&server_attr), sizeof(server_attr)) < 0)
	{
		throw std::runtime_error("ERROR binding");
	}
}

void Server::listenSock(int backlog)
{
	if (!(backlog > 1 && backlog < 20))
		throw std::runtime_error("backlock must be in 2 beetwen 20");

	if (listen(serverSockFD, backlog) < 0)
		throw std::runtime_error("Listening error");
}


void Server::acceptingRequest()
{
	//server is ready.
	cout << "Server is ready" << endl;
	while (1) // running değişkeni kullan
	{

		if (poll(pollfds.begin().base(), pollfds.size(), -1) < 0)
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