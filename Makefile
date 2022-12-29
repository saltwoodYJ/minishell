# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/10 18:30:36 by yejinam           #+#    #+#              #
#    Updated: 2022/12/29 16:06:46 by hyeokim2         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = #-fsanitize=address -g3 #-Wall -Wextra -Werror #

NAME = minishell

SRCS = minishell.c run_command.c parsing.c pipex_util.c ft_split.c make_redirect.c \
non_builtin.c builtin.c error.c

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
