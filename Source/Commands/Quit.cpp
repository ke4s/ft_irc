#include "../Library/Commands.hpp"

Quit::Quit(Server *server, bool authRequired) : Command(server, authRequired) {}

Quit::~Quit() {}

void Quit::execute(Client *client, std::vector<std::string> arguments) {

	std::string reason = arguments.empty() ? "Leaving..." : arguments.at(0);
	reason = reason.at(0) == ':' ? reason.substr(1) : reason;

	client->write(RPL_QUIT(client->getPrefix(), reason));
}