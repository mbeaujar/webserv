#.SILENT:
NAME   = webserv
CC     = clang++
RM     = rm -rf
CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = source/config/server

SRCS  =	main.cpp \
		config/directives/client_size.cpp \
		config/directives/error_page.cpp \
		config/directives/listen.cpp \
		config/directives/skip_utils.cpp \
		config/server/Location.cpp \
		config/server/Server.cpp \
		config/config_server.cpp \
		config/parse_server.cpp \
		config/parser.cpp 


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
	@mkdir -p $@/config/directives
	@mkdir -p $@/config/server

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)
	@$(RM) $(OBJS_DIRECTORY)

re : fclean all


.PHONY: all clean fclean re


