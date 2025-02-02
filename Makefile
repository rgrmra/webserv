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

RED					= \033[31m
GREEN				= \033[32m
YELLOW				= \033[33m
BLUE				= \033[34m
MAGENTA				= \033[35m
RESET				= \033[0m

NAME				= webserv

FILES				= $(shell find ./src -type f -name '*.cpp')

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
	@bash scripts/submodules.sh

test:
	@bash scripts/build_and_run_tests.sh
	$(call run_colorized_tests)

test_clean:
	@rm -rf build
	$(call print_color, $(RED), "Removing test library")

define run_colorized_tests
	GTEST_COLOR=1 ctest --test-dir build --output-on-failure -j12
endef

export TERM=xterm-256color

define print_color
    @echo "$(1)$(2)$(RESET)"
endef

.PHONY: 			all clean fclean re test test_clean sub
