# Section Settings

NAME		=	unit_test

SRC_DIR		=	../src/
SRC_LIB_NAME=	libwebserv.a
SRC_LIB		=	$(SRC_DIR)$(SRC_LIB_NAME)

TEST_DIR	=	unit_tests/
TEST_SRC	=	$(TEST_DIR)parseHeaderFieldsTests.cpp\
				$(TEST_DIR)parseRequestLineTests.cpp\
				$(TEST_DIR)processResponseTests.cpp\
				$(TEST_DIR)parseMimeTypesTests.cpp\
				$(TEST_DIR)uriParseTests.cpp\
				$(TEST_DIR)wildcardTests.cpp\
				$(TEST_DIR)routeTests.cpp\
				$(TEST_DIR)fileInfoTests.cpp


CC			=	clang++
CFLAGS		=	-Wall -Wextra -pedantic

SRC_LINKING	=	-I $(SRC_DIR)includes/\
				-I $(SRC_DIR)includes/autoIndex\
				-I $(SRC_DIR)includes/headerField\
				-I $(SRC_DIR)includes/observerAPI\
				-I $(SRC_DIR)includes/transportLayer\
				-I $(SRC_DIR)includes/cgi\
				-I $(SRC_DIR)includes/responses\
				-I $(SRC_DIR)includes/request\
				-I $(SRC_DIR)includes/methods\
				-I $(SRC_DIR)includes/config\
				-I $(SRC_DIR)includes/utility


# Section Rules

all: srcs $(NAME)

srcs:
	$(MAKE) -C $(SRC_DIR) $(SRC_LIB_NAME) -j5

$(NAME): $(TEST_SRC) $(SRC_LIB)
	$(CC) -D TEST=1 -o $(NAME) $(TEST_SRC) $(SRC_LIB) $(TEST_CFLAGS) $(SRC_LINKING) -L /usr/local/lib -lcriterion -I /usr/local/include -std=c++11 -Wl,-rpath=/usr/local/lib

x64_test: srcs $(SRC_LIB)
	$(CC) -D TEST=1 -o $(NAME) $(TEST_SRC) $(SRC_LIB) $(TEST_CFLAGS) $(SRC_LINKING) -lcriterion -I /usr/local/include -std=c++11 -Wl,-rpath=/usr/local/lib64

local_test: srcs $(TEST_SRC) $(SRC_LIB)
	$(CC) -D TEST=1 -o $(NAME) $(TEST_SRC) $(SRC_LIB) $(SRC_LINKING) -lcriterion -L ~/.brew/lib -I ~/.brew/include -std=c++11

clean:
	$(MAKE) -C $(SRC_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all srcs clean fclean re x64_test local_test acceptance
