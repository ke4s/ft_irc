#include "../../Includes/Server.hpp"


void Server::clientLOGIN(const vector<string>& splited_message, Client *recvClient)
{
	if (splited_message[0] == "PASS")
		Pass(splited_message, recvClient);
	else if (splited_message[0] == "NICK") //eksik
		Nick(splited_message, recvClient);
	else if (splited_message[0] == "USER")
		User(splited_message, recvClient);
}

void Server::clientEVENTS(vector<string> splited_message, Client* recvClient)
{
	if (splited_message[0] == "JOIN" && recvClient->_state == STATE_REGISTERED)
		Join(splited_message, recvClient);
}

void Server::clientMSG(vector<string> splited_message, string message, Client* recvClient)
{
	string dest = splited_message[1];

	clients_it dest_client;

	//birleşik halini de fonksiyonlara parametre olarak almak mantıklı olabilir.
	string messag = "";

	for (int i = 0; i < splited_message.size() ; ++i) {
		messag+= splited_message[i];
	}

	if (dest[0] == '&' || dest[0] == '#') //sending to channels
	{
		if (channels.find(dest) == channels.end())
			return ; // If channel is not exist

		multimap<string, Client*>::iterator members_it = channel_members.find(dest);
		while (members_it->first == dest)
		{
			if (members_it->second->_sockFD != recvClient->_sockFD)
				send(members_it->second->_sockFD, message.c_str(), message.length(), 0);
			members_it++;
		}
	}
	else if ((dest_client = findClientsByNick(dest)) != clients.end())
	{
		send(dest_client->second->_sockFD, message.c_str(), message.length(), 0);
	}
}
