#include "../../Includes/Server.hpp"

void    Server::Pass(const vector<string>& splited_message, Client *recvClient)
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