#.SILENT:
NAME=webserv
CC= clang++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM=rm -rf
HEADER =	

SRCS =	main.cpp \
		parsing/parser.cpp \
		parsing/server/Server.cpp \
		parsing/Location.cpp \
		parsing/find_server.cpp

OBJS = $(addprefix .objs/, $(SRCS:.cpp=.o))

SRCS_DIRECTORY=source/
OBJS_DIRECTORY=.objs/

$(OBJS_DIRECTORY)%.o : $(SRCS_DIRECTORY)%.cpp
	@$(CC) $(CFLAGS) $< -c -o $@


all : $(OBJS_DIRECTORY) $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


$(OBJS_DIRECTORY):
	@mkdir $@
	@mkdir -p $@/parsing/server

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)
	@$(RM) $(OBJS_DIRECTORY)

re : fclean all


.PHONY: all clean fclean re


