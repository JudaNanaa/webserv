# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: madamou <madamou@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/29 12:42:35 by madamou           #+#    #+#              #
#    Updated: 2024/11/09 19:38:24 by madamou          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



BLACK=	$(shell tput -Txterm setaf 0)
RED= 	$(shell tput -Txterm setaf 1)
GREEN= 	$(shell tput -Txterm setaf 2)
WHITE= 	$(shell tput -Txterm setaf 7)
YELLOW=	$(shell tput -Txterm setaf 3)
BLUE=	$(shell tput -Txterm setaf 6)
PURPLE=	$(shell tput -Txterm setaf 5)
END= 	$(shell tput -Txterm sgr0)

CC = c++

C_FLAGS = -Wall -Werror -Wextra -std=c++98 -Wshadow -Wno-shadow -g3

REQUEST = $(addprefix Request/, Request.cpp)

GLOBALDATA = $(addprefix GlobalData/, GlobalData.cpp)

CLIENT = $(addprefix Client/, Client.cpp)

SERVER = $(addprefix Server/, Server.cpp)

PARSER = $(addprefix Parser/, Parser.cpp ParserUtils.cpp Data.cpp Location.cpp)

UTILS = $(addprefix Utils/, utils.cpp)

CLASS = $(addprefix class/, RawBits.cpp $(PARSER) $(GLOBALDATA) $(CLIENT) $(SERVER) $(REQUEST) )

SRCS = main.cpp $(CLASS) $(UTILS)

SRCS_DIR = srcs/

OBJS_DIR = .objets/

SRCS := $(SRCS:%=$(SRCS_DIR)/%)

OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

DIR_DUP     = mkdir -p $(@D)

NAME =  webserv

G				= $(BLUE)
X 				= $(END)
BAR_SIZE		= 50

TOTAL_FILES		:= $(words $(SRCS))
COMPILED_FILES	:= 0

all : $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) $^ -o $@
	@echo
	@echo "✅$(BLUE)executable created >_$(END)✅"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@$(DIR_DUP)
	@$(CC) $(C_FLAGS) -c $< -o $@
	@$(eval COMPILED_FILES := $(shell echo $$(($(COMPILED_FILES)+1))))
	@echo -n ""
	@for i in `seq 1 $(shell echo "$$(($(COMPILED_FILES)*$(BAR_SIZE)/$(TOTAL_FILES)))")`; do \
		echo -n "$(G)▰$(X)" ; \
	done
	@for i in `seq 1 $(shell echo "$$(($(BAR_SIZE)-$(COMPILED_FILES)*$(BAR_SIZE)/$(TOTAL_FILES)))")`; do \
		echo -n "▱" ; \
	done
	@echo -n " ($(shell echo "scale=2; $(COMPILED_FILES)/$(TOTAL_FILES) * 100" | bc)%) "
	@echo -n ""
	@printf "%d/%d" $(COMPILED_FILES) $(TOTAL_FILES)
	@echo -n " "
	@printf "%s" $(notdir $<)
	@printf "\e[0K\r"

clean:
	@rm -rf $(OBJS_DIR)
	@echo "🧼🧼$(PURPLE)objects cleaned$(END)🧼🧼"

fclean: clean
	@rm -f $(NAME)
	@echo "🧼🧼$(PURPLE)executable cleaned$(END)🧼🧼"

re: fclean all

.PHONY: all clean fclean re
