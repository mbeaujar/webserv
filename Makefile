#.SILENT:
NAME   = webserv
CC     = clang++
RM     = rm -rf
CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = source/config/server

SRCS  =	main.cpp \
		config/directives/autoindex.cpp \
		config/directives/client_size.cpp \
		config/directives/error_page.cpp \
		config/directives/index.cpp \
		config/directives/listen.cpp \
		config/directives/method.cpp \
		config/directives/return.cpp \
		config/directives/root.cpp \
		config/directives/skip_utils.cpp \
		config/directives/fastcgi_param.cpp \
		config/server/Location.cpp \
		config/server/parse_location.cpp \
		config/server/parse_server.cpp \
		config/server/Server.cpp \
		config/parser.cpp \
		socket/connections.cpp \
		socket/create.cpp \
		socket/socket.cpp

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
	@mkdir -p $@/socket

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)
	@$(RM) $(OBJS_DIRECTORY)

re : fclean all


.PHONY: all clean fclean re


