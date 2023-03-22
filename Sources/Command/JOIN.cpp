#include "../../Includes/Server.hpp"

void Server::Join(const vector<string>& splited_message, Client *recvClient)
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