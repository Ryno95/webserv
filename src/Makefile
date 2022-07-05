# Section Settings

NAME		=	webserv
LIB			=	libwebserv.a

OBJ_DIR		=	objs/
SRC_DIR		=	srcs/
INCL_DIR	=	includes/

MAIN		=	$(SRC_DIR)main.cpp
CLASSES		=	Utility\
				Request\
				RequestParser\
				config/ParseTreeUtility\
				config/AppConfigParser\
				config/ServerConfigParser\
				config/HostConfigParser\
				config/LocationConfigParser\
				config/AppConfig\
				config/ServerConfig\
				config/HostConfig\
				config/LocationConfig\
				config/HostFields\
				methods/AMethod\
				methods/GETMethod\
				methods/POSTMethod\
				methods/DELETEMethod\
				methods/TargetInfo\
				responses/Response\
				responses/BadResponse\
				responses/RedirectResponse\
				responses/AutoIndexResponse\
				responses/CgiResponse\
				cgi/Cgi\
				cgi/Pipes\
				MimeTypes\
				Logger\
				Webserv\
				Client\
				Receiver\
				Sender\
				PollHandler\
				TickHandler\
				TimeoutHandler\
				Host\
				Uri\
				HeaderFields\
				HeaderFieldParser\
				autoIndex/HtmlBuilder\
				autoIndex/HtmlElement\
				autoIndex/AutoIndex

OBJS		=	$(CLASSES:%=$(OBJ_DIR)%.o)
HPPS		=	$(CLASSES:%=$(INCL_DIR)%.hpp)

CC			=	clang++
CFLAGS		=	-Wall -Wextra -pedantic
LINKING		=	-I $(INCL_DIR)


# Section Rules

all: $(NAME)

$(LIB): $(OBJS) $(HPPS)
	ar -r $(LIB) $(OBJS)

$(NAME): $(LIB) $(MAIN)
	$(CC) $(CFLAGS) $(LINKING) $(MAIN) $(LIB) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HPPS)
	@mkdir -p $(OBJ_DIR) $(@D)
	$(CC) $(CFLAGS) $(LINKING) -c $< -o $@

re: fclean all

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(LIB)

.PHONY: all clean fclean re
