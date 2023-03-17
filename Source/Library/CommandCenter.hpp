#pragma once

#include <string>
#include <numeric>
#include <regex>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "../Source/Library/Server.hpp"
#include "../Source/Library/Commands.hpp"
#include "../Source/Library/Client.hpp"
#include "../Source/Library/Channel.hpp"


class CommandCenter
{
    private:
        Server *server;
	    std::map<std::string, Commands *> commands;

    public:
        CommandCenter(Server *server);
        ~CommandCenter();
        void CommandCatcher(Client *client, const std::string &message);
};