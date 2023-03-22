NAME = ircserv

C++ = c++

FLAGS = -std=c++98

SRC_SERV =	main.cpp \
			Sources/Server/Server.Connection.cpp \
			Sources/Server/Server.Senders.cpp \
			Sources/Server/Server.CommandHandler.cpp \
			Sources/Server/Server.Setup.cpp \
			Sources/Server/Server.Utils.cpp \
			Sources/Command/NICK.cpp \
			Sources/Command/PASS.cpp \
			Sources/Command/USER.cpp \
			Sources/Command/JOIN.cpp \
			Sources/Command/PRIVMSG.cpp \
			Sources/Utils/Utils.cpp


SRC_BOT =	BotResources/main.cpp \
			BotResources/Bot.cpp

all:	$(SRC_SERV) $(SRC_BOT) ircserv bot

ircserv:	$(SRC_SERV)
	$(C++) $(FLAGS) $(SRC_SERV) -o ircserv

bot:	$(SRC_BOT)
	$(C++) $(FLAGS) $(SRC_BOT) -o bot


clean :
		rm -rf $(NAME) bot

fclean : clean
		@echo Cleaning %100

re : fclean all

.PHONY : all clean fclean re