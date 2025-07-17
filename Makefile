NAME = ircserv

CC = c++

FLAGS = -std=c++98 #-Wall -Werror -Wextra 

SRC = src/main.cpp src/parse.cpp server/server.cpp server/create.cpp client/client.cpp client/Channel.cpp \
	server/bot.cpp
HDR = headers/server.hpp headers/client.hpp headers/irc.hpp headers/channel.hpp

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	@$(CC) $(FLAGS) $^ -o $@
	@echo "\033[32mServer Is Ready✅👀"

%.o: %.cpp $(HDR) Makefile
	@$(CC) $(FLAGS) -c $< -o $@

clean :
	@rm -f $(OBJ)
	@echo "\033[0;33mCleaning Is Done🧼✨"

fclean : clean
	@rm -f $(NAME)
	@echo "\033[0;31mRemoving Is Done🗑 💔"

re : fclean all

.PHONY : clean