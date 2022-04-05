#.SILENT:
NAME   = webserv
CXX     = clang++
RM     = rm -rf
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 	
BONUS=0

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
		config/directives/max_body.cpp \
		config/directives/error_page.cpp \
		config/directives/cgi.cpp \
		config/directives/index.cpp \
		config/directives/listen.cpp \
		config/directives/method.cpp \
		config/directives/return.cpp \
		config/directives/root.cpp \
		config/directives/skip_utils.cpp \
		config/directives/upload.cpp \
		config/server/Location.cpp \
		config/server/s_method.cpp \
		config/server/s_port.cpp \
		config/server/Server.cpp \
		config/parse_location.cpp \
		config/parse_server.cpp \
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
SRCS_SUBDIRECTORY=config \
		config/directives \
		config/server \
		autoindex \
		cgi \
		method \
		request \
		response \
		socket
OBJS_DIRECTORY=.objs/

$(OBJS_DIRECTORY)%.o : $(SRCS_DIRECTORY)%.cpp
	@$(CXX) $(CXXFLAGS) -D BONUS=$(BONUS) $(HEADER) $< -c -o $@

all : $(NAME)

$(NAME) :  $(OBJS_DIRECTORY)  $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) $(HEADER) -lpthread -o $(NAME)

bonus: BONUS=1
bonus: all

$(OBJS_DIRECTORY):
	@mkdir $@ $(addprefix $@/, $(SRCS_SUBDIRECTORY))

path:
	@./.path.sh

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)
	@$(RM) $(OBJS_DIRECTORY)

re : fclean all


.PHONY: all clean fclean re
