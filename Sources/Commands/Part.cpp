#include "../../Includes/Commands.hpp"

Part::Part(Server *server) : Command(server) {}

Part::~Part() {}

void Part::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.empty()) {
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
		return;
	}

	std::string name = arguments[0];

	Channel *channel = _server->getChannel(name);
	if (!channel) {
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), name));
		return;
	}

	if (!client->getChannel() || client->getChannel()->getName() != name) {
		client->reply(ERR_NOTONCHANNEL(client->getNickname(), name));
		return;
	}

	client->leave();
}