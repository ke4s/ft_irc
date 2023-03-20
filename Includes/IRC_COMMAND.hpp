#pragma once

#define ERR_UNKNOWNCOMMAND(source, command)				"421 " + source + " " + command + " :Unknown command\n"
#define ERR_NEEDMOREPARAMS(source, command)				"461 " + source + " " + command + " :Not enough parameters\n"
#define ERR_NOTREGISTERED(source)						"451 " + source + " :You have not registered\n"
#define ERR_ALREADYREGISTERED(source)					"462 " + source + " :You may not reregister\n"
#define ERR_PASSWDMISMATCH(source)						"464 " + source + " :Password incorrect\n"
#define ERR_NONICKNAMEGIVEN(source)						"431 " + source + " :Nickname not given\n"
#define ERR_NICKNAMEINUSE(source)						"433 " + source + " " + source  + " :Nickname is already in use\n"
#define ERR_TOOMANYCHANNELS(source, channel)			"405 " + source + " " + channel + " :You have joined too many channels\n"
#define ERR_NOTONCHANNEL(source, channel)				"442 " + source + " " + channel + " :You're not on that channel\n"
#define ERR_NOSUCHCHANNEL(source, channel)				"403 " + source + " " + channel + " :No such channel\n"
#define ERR_BADCHANNELKEY(source, channel)				"475 " + source + " " + channel + " :Cannot join channel (+k)\n"
#define ERR_NOSUCHNICK(source, nickname)				"401 " + source + " " + nickname + " :No such nick/channel\n"
#define ERR_USERNOTINCHANNEL(source, nickname, channel)	"441 " + source + " " + nickname + " " + channel + " :They aren't on that channel\n"
#define ERR_CHANOPRIVSNEEDED(source, channel)			"482 " + source + " " + channel + " :You're not channel operator\n"
#define ERR_CHANNELISFULL(source, channel)				"471 " + source + " " + channel + " :Cannot join channel (+l)\n"
#define ERR_CANNOTSENDTOCHAN(source, channel)			"404 " + source + " " + channel + " :Cannot send to channel\n"

// NUMERIC REPLIES
#define RPL_WELCOME(source)						"001 " + source + " :Welcome " + source + " to the ft_irc network\n"
#define RPL_NAMREPLY(source, channel, users)	"353 " + source + " = " + channel + " :" + users ++ "\n"
#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list.\n"

// COMMAND REPLIES
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)					":" + source + " PART :" + channel
#define RPL_PING(source, token)						":" + source + " PONG :" + token
#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)			":" + source + " NOTICE " + target + " :" + message
#define RPL_QUIT(source, message)					":" + source + " QUIT :Quit: " + message
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args