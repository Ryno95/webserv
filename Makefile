# Section Settings

NAME		=	webserv
SRC_DIR		=	src/
SRCS		=	$(SRC_DIR)libwebserv.a
MAIN		=	$(SRC_DIR)srcs/main.cpp

CC			=	clang++
CFLAGS		=	-Wall -Wextra -pedantic
LINKING		=	-I src/includes/

# Section Rules

all: srcs $(NAME)

$(NAME): $(SRCS) $(MAIN)
	$(CC) -o $(NAME) $(MAIN) $(LINKING) $(CFLAGS) $(SRCS)

srcs:
	$(MAKE) -C $(SRC_DIR) -j5

run: $(NAME)
	./$(NAME)

re: fclean all

clean:
	$(MAKE) -C $(SRC_DIR) clean

fclean: clean
	rm -f $(NAME)

.PHONY: all clean fclean re run
