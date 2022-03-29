# Section Settings

NAME		?=	Webserver.out
TEST_NAME	=	unit_test

OBJ_DIR		=	objs/
SRC_DIR		=	srcs/
INCL_DIR	=	includes/
TEST_DIR	=	unit_tests/

MAIN		?=	$(SRC_DIR)main.cpp
CLASSES		=	Webserv\
				Client\
				Request

OBJS		=	$(CLASSES:%=$(OBJ_DIR)%.o)
HPPS		=	$(CLASSES:%=$(INCL_DIR)%.hpp)

CC			=	clang++
CFLAGS		=	-Wall -Wextra -pedantic
TEST_CFLAGS	=	$(CFLAGS) -Werror
LINKING		=	-I $(INCL_DIR)

TEST_SRC	=	$(TEST_DIR)test.cpp


# Section Rules

all: $(NAME)

$(NAME): $(OBJS) $(HPPS)
	$(CC) -o $(NAME) $(OBJS) $(MAIN) $(LINKING) $(CFLAGS)

test: $(TEST_SRC)
	$(CC) -o $(TEST_NAME) $(TEST_SRC) $(TEST_CFLAGS) -lcriterion -L /usr/local/lib -I /usr/local/include -std=c++11 -Wl,-rpath=/usr/local/lib

local_test: $(TEST_SRC)
	$(CC) -o $(TEST_NAME) $(TEST_SRC) $(TEST_CFLAGS) -lcriterion -L ~/.brew/lib -I ~/.brew/include -std=c++11
	./$(TEST_NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HPPS) # Need all HPPS here? Remakes all for a single HPP file change?
	$(CC) $(CFLAGS) $(LINKING) -c $< -o $@

$(OBJS): | $(OBJ_DIR) # pipe in recipe checks that it's only called once

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

re: fclean all

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TEST_NAME)
	rm -f $(NAME)

.PHONY: all clean fclean re test local_test
