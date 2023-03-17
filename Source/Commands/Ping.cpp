#include "../Library/Commands.hpp"

Ping::Ping(Server *server) : Command(server) {}

Ping::~Ping() {}

void Ping::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.empty()) {
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PING"));
		return;
	}

	client->write(RPL_PING(client->getPrefix(), arguments.at(0)));
}