#.SILENT:
NAME   = webserv
CC     = clang++
RM     = rm -rf
CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = -I source  \
	-I source/config/server \
	-I source/request \
	-I source/response \
	-I source/method \
	-I source/cgi \
	-I source/autoindex \
	-I source/socket 

SRCS  =	main.cpp \
		config/directives/autoindex.cpp \
		config/directives/client_size.cpp \
		config/directives/error_page.cpp \
		config/directives/cgi_ext.cpp \
		config/directives/path_cgi.cpp \
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
		config/server/Port.cpp \
		config/server/Server.cpp \
		config/parser.cpp \
		autoindex/Autoindex.cpp \
		cgi/Cgi.cpp \
		method/AMethods.cpp \
		method/Get.cpp \
		method/Post.cpp \
		method/Delete.cpp \
		request/Request.cpp \
		response/Response.cpp \
		socket/signal.cpp \
		socket/Socket.cpp \
		socket/Thread.cpp

OBJS = $(addprefix .objs/, $(SRCS:.cpp=.o))

SRCS_DIRECTORY=source/
OBJS_DIRECTORY=.objs/

$(OBJS_DIRECTORY)%.o : $(SRCS_DIRECTORY)%.cpp
	@$(CC) $(CFLAGS) $(HEADER) $< -c -o $@

all : $(OBJS_DIRECTORY) $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(HEADER) -lpthread -o $(NAME)

$(OBJS_DIRECTORY):
	@mkdir $@
	@mkdir -p $@/config/directives
	@mkdir -p $@/config/server
	@mkdir -p $@/autoindex
	@mkdir -p $@/cgi
	@mkdir -p $@/method
	@mkdir -p $@/request
	@mkdir -p $@/response
	@mkdir -p $@/socket

test:
	@netcat localhost 80 < netcat

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)
	@$(RM) $(OBJS_DIRECTORY)

re : fclean all


.PHONY: all clean fclean re
