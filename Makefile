NAME = ircserv

C++ = c++

FLAGS = -Wall -Werror -Wextra -std=c++98

SRC_SERV =	main.cpp \
			Sources/Server/Server.Connection.cpp \
			Sources/Server/Server.Senders.cpp \
			Sources/Server/Server.CommandHandler.cpp \
			Sources/Server/Server.Setup.cpp \
			Sources/Server/Server.Utils.cpp \


SRC_BOT =	BotResources/main.cpp \
			BotResources/Bot.cpp

all:	$(SRC_SERV) $(SRC_BOT) ircserv bot

ircserv:	$(SRC_SERV)
	c++ $(SRC_SERV) -o ircserv

bot:	$(SRC_BOT)
	c++ $(SRC_BOT) -o bot


clean :
		rm -rf $(NAME) bot

fclean : clean
		@echo Cleaning %100

re : fclean all

.PHONY : all clean fclean re