#.SILENT:
NAME=webserv
CC= clang++
CFLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDE= include
RM=rm -rf
HEADER = include/sample.hpp

SRCS = sample.cpp

OBJS = $(addprefix .objs/, $(SRCS:.cpp=.o))

SRCS_DIRECTORY=source/
OBJS_DIRECTORY=.objs/

$(OBJS_DIRECTORY)%.o : $(SRCS_DIRECTORY)%.cpp $(HEADER)
	@$(CC) $(CFLAGS) $< -I$(INCLUDE) -c -o $@


all : $(OBJS_DIRECTORY) $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $< -I$(INCLUDE) -o $@


$(OBJS_DIRECTORY):
	@mkdir $@

clean :
	@$(RM) $(OBJS)

fclean : clean
	@$(RM) $(NAME)
	@$(RM) $(OBJS_DIRECTORY)

re : fclean all


.PHONY: all clean fclean re


