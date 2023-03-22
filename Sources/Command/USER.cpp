#include "../../Includes/Server.hpp"

void Server::User(const vector<string>& splited_message, Client *recvClient)
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
		cout << inet_ntoa(recvClient->_sockATTR.sin_addr) << ":" << ntohs(recvClient->_sockATTR.sin_port) << ": USER SIGIN" << endl;
	}
	else
	{
		string already_registered = ERR_ALREADYREGISTERED(string("ircserv"));
		send(recvClient->_sockFD, already_registered.c_str(), already_registered.length(), 0);
	}
}