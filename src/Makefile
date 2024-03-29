# Section Settings

NAME		=	webserv
LIB			=	libwebserv.a

OBJ_DIR		=	objs/
SRC_DIR		=	srcs/
INCL_DIR	=	includes/

MAIN		=	$(SRC_DIR)main.cpp
CLASSES		=	utility/Utility\
				utility/FileInfo\
				utility/Logger\
				utility/Uri\
				request/Request\
				request/RequestParser\
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
				config/MimeTypes\
				methods/AMethod\
				methods/GETMethod\
				methods/POSTMethod\
				methods/DELETEMethod\
				responses/Response\
				responses/BadResponse\
				responses/RedirectResponse\
				responses/AutoIndexResponse\
				responses/CgiResponse\
				cgi/Cgi\
				cgi/Executable\
				cgi/Pipes\
				transportLayer/Webserv\
				transportLayer/Client\
				transportLayer/Receiver\
				transportLayer/Sender\
				transportLayer/Host\
				observerAPI/PollHandler\
				observerAPI/TickHandler\
				observerAPI/TimeoutHandler\
				headerField/HeaderFields\
				headerField/HeaderFieldParser\
				autoIndex/HtmlBuilder\
				autoIndex/HtmlElement\
				autoIndex/AutoIndex

OBJS		=	$(CLASSES:%=$(OBJ_DIR)%.o)
HPPS		=	$(CLASSES:%=$(INCL_DIR)%.hpp)

CC			=	clang++
CFLAGS		=	-Wall -Wextra -pedantic
LINKING		=	-I $(INCL_DIR)\
				-I $(INCL_DIR)autoIndex\
				-I $(INCL_DIR)headerField\
				-I $(INCL_DIR)observerAPI\
				-I $(INCL_DIR)transportLayer\
				-I $(INCL_DIR)cgi\
				-I $(INCL_DIR)responses\
				-I $(INCL_DIR)request\
				-I $(INCL_DIR)methods\
				-I $(INCL_DIR)config\
				-I $(INCL_DIR)utility


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
