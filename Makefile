NAME = ircserv

C++ = g++

FLAGS = -Wall -Werror -Wextra -std=c++98

SRC = $(shell find . -type f -name "*.cpp")

OBJ = $(SRC:.cpp=.o)


all : $(NAME)

$(NAME)	: $(OBJ)
		$(C++) $(FLAGS) $(OBJ) -o $(NAME)

clean :
		rm -rf $(OBJ)

fclean : clean
		rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re