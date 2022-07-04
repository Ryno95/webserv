# Section Settings

NAME		=	unit_test

SRC_DIR		=	../src/
SRCS		=	$(SRC_DIR)libwebserv.a

TEST_DIR	=	unit_tests/
TEST_SRC	=	$(TEST_DIR)parseHeaderFieldsTests.cpp\
				$(TEST_DIR)parseRequestLineTests.cpp\
				$(TEST_DIR)processResponseTests.cpp\
				$(TEST_DIR)parseMimeTypesTests.cpp\
				$(TEST_DIR)uriParseTests.cpp\
				$(TEST_DIR)wildcardTests.cpp\
				$(TEST_DIR)routeTests.cpp\
				$(TEST_DIR)prependTests.cpp


CC			=	clang++
CFLAGS		=	-Wall -Wextra -pedantic
SRC_LINKING	=	-I $(SRC_DIR)includes/

# Section Rules

all: srcs $(NAME)

srcs:
	$(MAKE) -C $(SRC_DIR) lib -j5

$(NAME): $(TEST_SRC) $(SRCS)
	$(CC) -D TEST=1 -o $(NAME) $(TEST_SRC) $(SRCS) $(TEST_CFLAGS) $(SRC_LINKING) -L /usr/local/lib -lcriterion -I /usr/local/include -std=c++11 -Wl,-rpath=/usr/local/lib

x64_test: srcs $(SRCS)
	$(CC) -D TEST=1 -o $(NAME) $(TEST_SRC) $(SRCS) $(TEST_CFLAGS) $(SRC_LINKING) -lcriterion -I /usr/local/include -std=c++11 -Wl,-rpath=/usr/local/lib64

local_test: srcs $(TEST_SRC) $(SRCS)
	$(CC) -D TEST=1 -o $(NAME) $(TEST_SRC) $(SRCS) $(SRC_LINKING) -lcriterion -L ~/.brew/lib -I ~/.brew/include -std=c++11

acceptance: $(NAME)
	./acceptance_tests/acceptance.sh

clean:
	$(MAKE) -C $(SRC_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all srcs clean fclean re x64_test local_test acceptance
