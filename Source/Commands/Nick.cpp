#include "../Library/Commands.hpp"

Nick::Nick(Server *server, bool auth) : Command(server, auth) {}

Nick::~Nick() {}

void Nick::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.empty() || arguments[0].empty()) {
		client->reply(ERR_NONICKNAMEGIVEN(client->getNickname()));
		return;
	}

	std::string nickname = arguments[0];

	if (_server->getClient(nickname)) {
		client->reply(ERR_NICKNAMEINUSE(client->getNickname()));
		return;
	}
	client->setNickname(nickname);
	client->welcome();
}