#include "../../Includes/Server.hpp"


void Server::clientLOGIN(const vector<string>& splited_message, Client *recvClient)
{
	if (splited_message[0] == "PASS")
	{
		if (splited_message.size() < 2 && recvClient->_state == STATE_PASS)
		{
			string need_more_params = ERR_NEEDMOREPARAMS(string("ircserv"), splited_message[0]);
			send(recvClient->_sockFD, need_more_params.c_str(), need_more_params.length(), 0);
		}
		else if (recvClient->_state == STATE_PASS)
		{
			string recv_password = splited_message[1];
			if (recv_password == password)
			{
				recvClient->_state = STATE_NICK;
				cout << inet_ntoa(recvClient->_sockATTR.sin_addr) << ":" << ntohs(recvClient->_sockATTR.sin_port) << ": PASSED" << endl;
			}
		}
		else
		{
			string already_registered = ERR_ALREADYREGISTERED(string("ircserv"));
			send(recvClient->_sockFD, already_registered.c_str(), already_registered.length(), 0);
		}
	}
	else if (splited_message[0] == "NICK") //eksik
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
	else if (splited_message[0] == "USER")
	{
		if (splited_message.size() < 5 && recvClient->_state == STATE_USER)
		{
			string need_more_params = ERR_NEEDMOREPARAMS(string("ircserv"), splited_message[0]);
			send(recvClient->_sockFD, need_more_params.c_str(), need_more_params.length(), 0);
		}
		else if (recvClient->_state == STATE_USER)
		{
			recvClient->_hostName = splited_message[2];
			recvClient->_serverName = splited_message[3];
			recvClient->_fullName = splited_message[4];
			recvClient->_state = STATE_REGISTERED;
			recvClient->_isRegistered = 1;
			cout << inet_ntoa(recvClient->_sockATTR.sin_addr) << ":" << ntohs(recvClient->_sockATTR.sin_port) << ": PASSED" << endl;
		}
		else
		{
			string already_registered = ERR_ALREADYREGISTERED(string("ircserv"));
			send(recvClient->_sockFD, already_registered.c_str(), already_registered.length(), 0);
		}
	}
}

void Server::clientEVENTS(vector<string> splited_message, Client* recvClient)
{
	if (splited_message[0] == "JOIN" && recvClient->_state == STATE_REGISTERED)
	{
		if (splited_message.size() < 2)
		{
			string need_more_params = ERR_NEEDMOREPARAMS(string("ircserv"), splited_message[0]);
			send(recvClient->_sockFD, need_more_params.c_str(), need_more_params.length(), 0);
		}
		else
		{
			string channel_name = splited_message[1];
			if (channels.find(channel_name) != channels.end())
			{
				//Add recvClient to the channel as a member
				channel_members.insert(make_pair(channel_name, recvClient));
			}
			else
			{
				//Added New Channel and set recvClient to Admin.
				channels.insert(make_pair(channel_name, make_pair(recvClient, string(""))));
				channel_members.insert(make_pair(channel_name, recvClient));
			}
		}
	}
}

void Server::clientMSG(vector<string> splited_message, Client* recvClient)
{
	string dest = splited_message[1];

	clients_it dest_client;

	//birleşik halini de fonksiyonlara parametre olarak almak mantıklı olabilir.
	string message = "";
	for (int i = 0; i < splited_message.size() ; ++i) {
		message+= splited_message[i];
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
