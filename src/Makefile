# Section Settings

NAME		=	libwebserv.a

OBJ_DIR		=	objs/
SRC_DIR		=	srcs/
INCL_DIR	=	includes/

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
				MimeTypes\
				Logger\
				Webserv\
				Client\
				Receiver\
				Sender\
				Cgi\
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

$(NAME): $(OBJS) $(HPPS)
	ar -r $(NAME) $(OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HPPS) # Need all HPPS here? Remakes all for a single HPP file change?
	@mkdir -p $(OBJ_DIR) $(@D)
	$(CC) $(CFLAGS) $(LINKING) -c $< -o $@

re: fclean all

clean:
	rm -rf $(OBJ_DIR)

fclean: clean

.PHONY: all clean fclean re
