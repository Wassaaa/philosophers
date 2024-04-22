# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/22 13:54:57 by aklein            #+#    #+#              #
#    Updated: 2024/04/23 01:00:02 by aklein           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
# COMPILATION
################################################################################
CC				=	cc
CC_STRICT		=	-Wall -Wextra -Werror
CC_DEBUG		=	-g #-fsanitize=leak
HEADERS			=	-I $(INCLUDES)
CC_FULL			=	$(CC) $(CC_STRICT) $(HEADERS) $(CC_DEBUG)
OBJ_DIR			=	./obj

################################################################################
# MANDATORY
################################################################################
NAME			=	philo
M_SRC_DIR		=	.
INCLUDES		=	.
M_HEADER		=	$(INCLUDES)/philo.h
M_OBJECTS		=	$(addprefix $(OBJ_DIR)/, $(M_SRCS:%.c=%.o))
M_SRCS			=	philo.c ft_atoi.c

################################################################################
# RULES
################################################################################

vpath %.c $(M_SRC_DIR)

all: $(NAME)

$(NAME): $(M_OBJECTS)
	$(CC_FULL) $(M_OBJECTS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c $(M_HEADER)
		@mkdir -p $(@D)
		$(CC_FULL) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(NAME).dSYM/


fclean: clean
	rm -f $(NAME)

re: fclean all

################################################################################
# NORM
################################################################################
norm:
	norminette . $(LIBFT_DIR) | grep -v "OK!" || true

norm2:
	norminette . $(LIBFT_DIR)

################################################################################
# PHONY
################################################################################
.PHONY: all bonus re clean fclean
