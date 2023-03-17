#include "../Library/Commands.hpp"

Pong::Pong(Server *server) : Command(server) {}

Pong::~Pong() {}

void Pong::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.empty()) {
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PONG"));
		return;
	}

	client->write(RPL_PING(client->getPrefix(), arguments.at(0)));
}