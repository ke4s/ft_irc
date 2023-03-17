NAME = ircserv

C++ = g++

FLAGS = -Wall -Werror -Wextra -std=c++98

#SRC = $(shell find . -type f -name "*.cpp")

SRC_SERV = main.cpp

SRC_BOT = Bot/Bot.cpp

all: ircserv botp

ircserv: $(SRC_SERV)
	c++ $(SRC_SERV) -o ircserv

botp: $(SRC_BOT)
	c++ $(SRC_BOT) -o botp

#OBJ = $(SRC:.cpp=.o)


#all : $(NAME)

#$(NAME)	: $(OBJ)
#		$(C++) $(FLAGS) $(OBJ) -o $(NAME)

#clean :
#		rm -rf $(OBJ)


re : fclean all

.PHONY : all clean fclean re botp