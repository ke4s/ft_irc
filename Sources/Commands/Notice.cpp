#include "../../Includes/Commands.hpp"


Notice::Notice(Server *server) : Command(server) {};

Notice::~Notice() {};

void Notice::execute(Client *client, std::vector<std::string> arguments) {

	if (arguments.size() < 2 || arguments[0].empty() || arguments[1].empty()) {
		return;
	}

	std::string target = arguments.at(0);
	std::string message;

	for (std::vector<std::string>::iterator it = arguments.begin() + 1; it != arguments.end(); it++) {
		message.append(*it + " ");
	}

	message = message.at(0) == ':' ? message.substr(1) : message;

	if (target.at(0) == '#') {

		Channel *channel = client->getChannel();
		if (!channel) {
			return;
		}

		if (channel->isNoExt()){
			std::vector<std::string>			nicknames(channel->getNicknames());
			std::vector<std::string>::iterator	i;
			
			for (i = nicknames.begin(); i != nicknames.end(); i++)
				if (*i == client->getNickname())
					break;
			if (i == nicknames.end())
			{
				client->reply(ERR_CANNOTSENDTOCHAN(client->getNickname(), target));
				return;
			}
		}

		channel->broadcast(RPL_NOTICE(client->getPrefix(), target, message), client);
		return;
	}

	Client *dest = _server->getClient(target);
	if (!dest) {
		return;
	}

	dest->write(RPL_NOTICE(client->getPrefix(), target, message));
}