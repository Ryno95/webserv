# Section Settings

NAME		=	Webserver.out
TEST_NAME	=	unit_test

OBJ_DIR		=	objs/
SRC_DIR		=	srcs/
INCL_DIR	=	includes/
TEST_DIR	=	unit_tests/

MAIN		?=	$(SRC_DIR)main.cpp
CLASSES		=	Utility\
				config/ParseTreeUtility\
				config/ConfigParser\
				config/ServerConfigParser\
				config/HostConfigParser\
				config/LocationConfigParser\
				config/AppConfig\
				config/ServerConfig\
				config/HostConfig\
				config/LocationConfig\
				MimeTypes\
				Logger
				# Webserv\
				# Client\
				# Request\
				# Receiver\
				# Sender\
				# AMethod\
				# GETMethod\
				# POSTMethod\
				# DELETEMethod\
				# PollHandler\
				# TickHandler\
				# TimeoutHandler\
				# Host\
				# responses/Response\
				# responses/OkStatusResponse\
				# responses/BadStatusResponse\
				# responses/RedirectResponse\
				# Uri\
				# HeaderFields


OBJS		=	$(CLASSES:%=$(OBJ_DIR)%.o)
HPPS		=	$(CLASSES:%=$(INCL_DIR)%.hpp)

CC			=	clang++
CFLAGS		=	-Wall -Wextra -pedantic
TEST_CFLAGS	=	$(CFLAGS)
LINKING		=	-I $(INCL_DIR)

TEST_SRC	=	$(TEST_DIR)parseHeaderFieldsTests.cpp\
				$(TEST_DIR)parseRequestLineTests.cpp\
				$(TEST_DIR)processResponseTests.cpp\
				$(TEST_DIR)parseMimeTypesTests.cpp\
				$(TEST_DIR)uriParseTests.cpp\
				$(TEST_DIR)wildcardTests.cpp\
				$(TEST_DIR)routeTests.cpp


# Section Rules

all: $(NAME)

$(NAME): $(OBJS) $(HPPS) $(MAIN)
	$(CC) -o $(NAME) $(OBJS) $(MAIN) $(LINKING) $(CFLAGS)

test: $(TEST_SRC) $(OBJS)
	$(CC) -D TEST=1 -o $(TEST_NAME) $(TEST_SRC) $(OBJS) $(TEST_CFLAGS) $(LINKING) -L /usr/local/lib -lcriterion -I /usr/local/include -std=c++11 -Wl,-rpath=/usr/local/lib

pascal_test: $(TEST_SRC) $(OBJS)
	$(CC) -D TEST=1 -o $(TEST_NAME) $(TEST_SRC) $(OBJS) $(TEST_CFLAGS) $(LINKING) -lcriterion -I /usr/local/include -std=c++11 -Wl,-rpath=/usr/local/lib64

local_test: $(TEST_SRC) $(OBJS)
	$(CC) -D TEST=1 -o $(TEST_NAME) $(TEST_SRC) $(OBJS) $(LINKING) -lcriterion -L ~/.brew/lib -I ~/.brew/include -std=c++11
	./$(TEST_NAME)

acceptance: $(NAME)
	./acceptance_tests/acceptance.sh

run: $(NAME)
	./$(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HPPS) # Need all HPPS here? Remakes all for a single HPP file change?
	@mkdir -p $(OBJ_DIR) $(@D)
	$(CC) $(CFLAGS) $(LINKING) -c $< -o $@

re: fclean all

clean:
	rm -rf $(OBJ_DIR)
	rm -rf root/uploads/*.txt

fclean: clean
	rm -f $(TEST_NAME)
	rm -f $(NAME)

.PHONY: all clean fclean re test local_test run
