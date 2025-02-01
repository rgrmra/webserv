# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 17:31:36 by rde-mour          #+#    #+#              #
#    Updated: 2025/01/04 17:10:40 by rde-mour         ###   ########.org.br    #
#                                                                              #
# **************************************************************************** #

RED					= $(shell tput setaf 1)
GREEN				= $(shell tput setaf 2)
YELLOW				= $(shell tput setaf 3)
BLUE				= $(shell tput setaf 4)
MAGENTA				= $(shell tput setaf 5)
RESET				= $(shell tput sgr0)

NAME				= webserv

FILES				= $(shell find . -type f -name '*.cpp')

OBJS				= $(FILES:%.cpp=%.o)

HEADERS				= $(addprefix -I, $(shell find . -type d))

COMPILER			= c++

CFLAGS				= -Wall -Wextra -Werror -std=c++98 -g3

all: 				$(NAME)

$(NAME):			$(OBJS)
					@$(COMPILER) $(CFLAGS) $(OBJS) -o $(NAME)
					@echo "$(BLUE)Compiled $(NAME) successfully$(RESET)"

%.o: 				%.cpp
					@$(COMPILER) $(CFLAGS) -c $< -o $@ $(HEADERS)
					@echo "$(GREEN)Compiled$(RESET) $(notdir $<)"

clean:
					@echo "$(RED)Removing$(RESET) $(NAME) objects"
					@rm -rf $(OBJS)

fclean: 			clean
					@echo "$(RED)Removing$(RESET) $(NAME)"
					@rm -rf $(NAME)

re:					fclean all

sub:
	bash scripts/submodules.sh

.PHONY: 			all clean fclean re
