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
		config/directives/fastcgi_extension.cpp \
		config/directives/fastcgi.cpp \
		config/directives/index.cpp \
		config/directives/listen.cpp \
		config/directives/method.cpp \
		config/directives/return.cpp \
		config/directives/root.cpp \
		config/directives/skip_utils.cpp \
		config/directives/upload.cpp \
		config/server/Location.cpp \
		config/server/s_method.cpp \
		config/server/parse_location.cpp \
		config/server/parse_server.cpp \
		config/server/Server.cpp \
		config/parser.cpp \
		socket/request/parser.cpp \
		socket/request/Request.cpp \
		socket/response/autoindex.cpp \
		socket/response/call_cgi.cpp \
		socket/response/create.cpp \
		socket/response/get.cpp \
		socket/response/parse_cgi.cpp \
		socket/response/post.cpp \
		socket/response/header.cpp \
		socket/response/utils.cpp \
		socket/connections.cpp \
		socket/create.cpp \
		socket/socket.cpp \
		socket/Thread.cpp 

OBJS = $(addprefix .objs/, $(SRCS:.cpp=.o))

SRCS_DIRECTORY=source/
OBJS_DIRECTORY=.objs/

$(OBJS_DIRECTORY)%.o : $(SRCS_DIRECTORY)%.cpp
	@$(CC) $(CFLAGS) $< -D LIMIT=-1 -c -o $@


all : $(OBJS_DIRECTORY) $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -lpthread -o $(NAME)

$(OBJS_DIRECTORY):
	@mkdir $@
	@mkdir -p $@/config/directives
	@mkdir -p $@/config/server
	@mkdir -p $@/socket/request
	@mkdir -p $@/socket/response

test:
	@netcat localhost 80 < netcat

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)
	@$(RM) $(OBJS_DIRECTORY)

re : fclean all


.PHONY: all clean fclean re


