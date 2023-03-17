#pragma once

#include <string>
#include <numeric>
#include <regex>

class Commands
{
	protected:
		Server *server;
		bool control;

	public:
		explicit Commands(Server *server, bool control = true) : server(server), control(control){};
		virtual ~Commands(){};

		bool control() const { return control; };

		virtual void execute(Client *client, std::vector<std::string> arguments) = 0;
};

class Notice : public Commands
{
	public:
		Notice(Server *server);
		~Notice();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Privmsg : public Commands
{
	public:
		Privmsg(Server *server);
		~Privmsg();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Part : public Commands
{
	public:
		Part(Server *server);
		~Part();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Quit : public Commands
{
	public:
		Quit(Server *server, bool authRequired);
		~Quit();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Join : public Commands
{
	public:
		Join(Server *server);
		~Join();

		void execute(Client *client, std::vector<std::string> arguments);
};

class User : public Commands
{
	public:
		User(Server *server, bool authRequired);
		~User();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Nick : public Commands
{
	public:
		Nick(Server *server, bool authRequired);
		~Nick();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Pass : public Commands
{
	public:
		Pass(Server *server, bool authRequired);
		~Pass();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Kick : public Commands
{
	public:
		Kick(Server *server);
		~Kick();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Ping : public Commands
{
	public:
		Ping(Server *server);
		~Ping();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Pong : public Commands
{
	public:
		Pong(Server *server);
		~Pong();

		void execute(Client *client, std::vector<std::string> arguments);
};

class Mode : public Commands
{
	public:
		Mode(Server *server);
		~Mode();

		void execute(Client *client, std::vector<std::string> arguments);
};
