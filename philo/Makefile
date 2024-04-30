# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/22 13:54:57 by aklein            #+#    #+#              #
#    Updated: 2024/04/30 10:49:56 by aklein           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
# COMPILATION
################################################################################
CC				=	cc
CC_STRICT		=	-Wall -Wextra -Werror -pthread
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
M_SRCS			=	philo.c ft_atoi.c args.c clean.c forks.c init.c \
					print.c routine_utils.c routine.c threads.c putstr.c

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
# VALGRIND
################################################################################

CC_VG = $(CC) $(CC_STRICT) $(HEADERS)

VG = valgrind

HG_FLAGS = --tool=helgrind #--suppressions=sup #--gen-suppressions=all --log-file=helgrind.log

VG_FLAGS = --leak-check=full \
	--show-leak-kinds=all \
	--trace-children=yes

VG_LOG_FLAGS = $(VG_FLAGS) \
	--log-file=$(VG_LOG) \
	--track-origins=yes \
	--verbose
VG_LOG = valgrind_leaks.log

#VG_ARGS = 2 100 200 200
#VG_ARGS = 2 100 200 200 4
#VG_ARGS = 1 100 200 200
#VG_ARGS = 1 100 200 200 4
VG_ARGS = 6 800 200 200 6
#VG_ARGS = 4 210 100 100 3
#VG_ARGS = 5 210 100 100 3
#VG_ARGS = 5 410 100 200 3
#VG_ARGS = 100 410 100 200 3
#VG_ARGS = 99 410 100 200 3

hg: vg_build
	$(VG) $(HG_FLAGS) ./$(NAME) $(VG_ARGS)

vg: vg_build
	$(VG) $(VG_FLAGS) ./$(NAME) $(VG_ARGS)

vglog: vg_build
	$(VG) $(VG_LOG_FLAGS) ./$(NAME) $(VG_ARGS)

vg_build: $(M_OBJECTS)
	$(CC_FULL) $(M_OBJECTS) -o $(NAME)

vglog_clean: fclean
	rm -f $(VG_LOG)

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
