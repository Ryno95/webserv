NAME=project
TEST_NAME=unit_test
CC=clang
CFLAGS=-Wall -Wextra -Werror

TEST_FILES=				\
	unit_tests/test.cpp	\

all: $(NAME)

$(NAME):
	echo "Hello"

test: $(TEST_FILES)
	clang++ -o test $(TEST_FILES) -lcriterion -L /usr/local/lib -I /usr/local/include -std=c++11 -Wl,-rpath=/usr/local/lib

rhino_test: $(TEST_FILES)
	clang++ -o test $(TEST_FILES) -lcriterion -L /Users/rmeiboom/.brew/lib -I /Users/rmeiboom/.brew/include -std=c++11 -o $(TEST_NAME)
	./unit_test

psp_test: $(TEST_FILES)
	clang++ -o test $(TEST_FILES) -lcriterion -L /Users/pspijkst/.brew/lib -I /Users/pspijkst/.brew/include -std=c++11 -o $(TEST_NAME)
	./unit_test

re: fclean all

clean:
	rm -f $(NAME)
	rm -f *.o

fclean: clean

.PHONY: all clean fclean re test rhino_test psp_test