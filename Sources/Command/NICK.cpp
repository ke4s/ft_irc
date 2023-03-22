#include "../../Includes/Server.hpp"

void Server::Nick(const vector<string>& splited_message, Client *recvClient)
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
			send(recvClient->_sockFD, "You can't NICK", 15, 0);
}