# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/10 18:30:36 by yejinam           #+#    #+#              #
#    Updated: 2023/01/06 22:33:18 by hyeokim2         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -fsanitize=address -g3 #-Wall -Wextra -Werror 

NAME = minishell
PARSE_DIR = ./parse/

SRCS = minishell.c run_command.c pipex_util.c ft_split.c make_redirect.c \
non_builtin.c builtin.c error.c ft_strncmp.c libft_func.c \
ft_strchr.c ft_strdup.c \
$(PARSE_DIR)parse_cmd.c \
$(PARSE_DIR)parse_envp.c \
$(PARSE_DIR)parse_red.c \
$(PARSE_DIR)parsing_utils.c \
$(PARSE_DIR)tokenize.c \
 #parsing.c  tokenize.c  parse_envp.c  parsing_utils.c  pipe.c \


OBJS = $(SRCS:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(<:.c=.o) 

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -lreadline -o $(NAME)

clean :
	rm -rf $(OBJS) $(BONUS_OBJS)

fclean : clean
	rm -rf $(NAME) $(BONUS_NAME)

re : fclean
	$(MAKE) all

.PHONY : all clean fclean re
