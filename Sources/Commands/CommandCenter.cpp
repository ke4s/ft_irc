#include "../../Includes/CommandCenter.hpp"

CommandCenter::CommandCenter(Server *server) : server(server)
{
	commands["PASS"] = new Pass(server, false);
	commands["NICK"] = new Nick(server, false);
	commands["USER"] = new User(server, false);
	commands["QUIT"] = new Quit(server, false);

	commands["PING"] = new Ping(server);
	commands["PONG"] = new Pong(server);
	commands["JOIN"] = new Join(server);
	commands["MODE"] = new Mode(server);
	commands["PART"] = new Part(server);
	commands["KICK"] = new Kick(server);

	commands["PRIVMSG"] = new Privmsg(server);
	commands["NOTICE"] = new Notice(server);
}

CommandCenter::~CommandCenter()
{
	for (std::map<std::string, Commands *>::iterator it = commands.begin(); it != commands.end(); it++)
		delete it->second;
	commands.clear();
}

void CommandCenter::CommandCatcher(Client *client, const std::string &message)
{

	std::stringstream ssMessage(message);
	std::string syntax;

	while (std::getline(ssMessage, syntax))
	{

		syntax = syntax.substr(0, syntax[syntax.length() - 1] == '\r' ? syntax.length() - 1 : syntax.length());
		std::string name = syntax.substr(0, syntax.find(' '));

		try
		{
			Commands *command = commands.at(name);

			std::vector<std::string> arguments;

			std::string buf;
			std::stringstream ss(syntax.substr(name.length(), syntax.length()));

			while (ss >> buf)
			{
				arguments.push_back(buf);
			}

			if (!client->isRegistered() && command->control())
			{
				client->reply(ERR_NOTREGISTERED(client->getNickname()));
				return;
			}
			command->execute(client, arguments);
		}
		catch (const std::out_of_range &e)
		{
			client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), name));
		}
	}
}