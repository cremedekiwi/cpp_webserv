# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 21:31:05 by ale-tell          #+#    #+#              #
#    Updated: 2025/02/27 12:48:34 by jarumuga         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = clang++

CDIR = src
HDIR = include
ODIR =	obj

ODIRS = obj/utils

SRC =	$(CDIR)/CGI.cpp \
		$(CDIR)/ConfigParser.cpp \
		$(CDIR)/Cookie.cpp \
		$(CDIR)/Exceptions.cpp \
		$(CDIR)/Listing.cpp \
		$(CDIR)/main.cpp \
		$(CDIR)/Request.cpp \
		$(CDIR)/Response.cpp \
		$(CDIR)/Server.cpp \
		$(CDIR)/SuperServer.cpp \
		$(CDIR)/SessionManager.cpp \
		$(CDIR)/utils/color.cpp \
		$(CDIR)/utils/cookie.cpp \
		$(CDIR)/utils/ft_inet_addr.cpp \
		$(CDIR)/utils/ft_isnum.cpp \
		$(CDIR)/utils/ft_split.cpp \
		$(CDIR)/utils/ft_strtrim.cpp \
		$(CDIR)/utils/http_codes.cpp \
		$(CDIR)/utils/mimes.cpp \
		$(CDIR)/utils/time_stamp.cpp \
		$(CDIR)/utils/urlEncoding.cpp

### Compile ###
OBJ = $(patsubst $(CDIR)/%.cpp, $(ODIR)/%.o, $(SRC))
CFLAGS = -g3 -std=c++98 -I $(HDIR) -MMD
CFLAGS += -Wall -Wextra -Werror
# CFLAGS = -fsanitize=address,leak,undefined,pointer-overflow

all: $(NAME)

$(ODIR)/%.o: $(CDIR)/%.cpp | $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) | $(ODIR)
	$(CC) $^ $(CFLAGS) -o $@

test: all
	./$(NAME) ./webserv.conf

vtest: all
	valgrind --track-origins=yes ./$(NAME) ./webserv.conf

fdtest: all
	valgrind --track-fds=yes --trace-children=yes ./$(NAME) ./webserv.conf

$(ODIR):
	mkdir -p $@
	mkdir -p $(ODIRS)

init:
	mkdir -p $(HDIR) $(CDIR) $(ODIR)

clean:
	rm -rf $(ODIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

compile_command.json:
	bear -- make re

.PHONY: clean fclean all re test vtest init
