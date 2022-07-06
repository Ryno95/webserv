# Section Settings

NAME		=	webserv
LIB			=	libwebserv.a

OBJ_DIR		=	objs/
SRC_DIR		=	srcs/
INCL_DIR	=	includes/

MAIN		=	$(SRC_DIR)main.cpp
CLASSES		=	utils/Utility\
				requests/Request\
				requests/RequestParser\
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
				logger/Logger\
				Webserv\
				Client\
				Receiver\
				Sender\
				observerAPI/PollHandler\
				observerAPI/TickHandler\
				observerAPI/TimeoutHandler\
				Host\
				utils/Uri\
				headerFields/HeaderFields\
				headerFields/HeaderFieldParser\
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
