# Section Settings

SRCS		=	../src/libwebserv.a

TEST_SRC	=	$(TEST_DIR)parseHeaderFieldsTests.cpp\
				$(TEST_DIR)parseRequestLineTests.cpp\
				$(TEST_DIR)processResponseTests.cpp\
				$(TEST_DIR)parseMimeTypesTests.cpp\
				$(TEST_DIR)uriParseTests.cpp\
				$(TEST_DIR)wildcardTests.cpp\
				$(TEST_DIR)routeTests.cpp\
				$(TEST_DIR)prependTests.cpp


# Section Rules

$(NAME): all

all:

clean:
	$(MAKE) -C $(SRC_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all
