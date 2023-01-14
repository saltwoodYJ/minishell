# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/10 18:30:36 by yejinam           #+#    #+#              #
#    Updated: 2023/01/14 14:36:40 by hyeokim2         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=address 

NAME = minishell

PARSE_DIR = ./parse/

BUILTIN_DIR = ./builtin/

EXEC_DIR = ./exec/

MAIN_DIR = ./main/

NONBUILTIN_DIR = ./non_builtin/

REDIR_DIR = ./redirect/

UTILS_DIR = ./utils/

HEADER_DIR = ./header/

SRCS = $(MAIN_DIR)minishell.c \
$(PARSE_DIR)parse_cmd.c $(PARSE_DIR)parse_envp.c $(PARSE_DIR)parse_red.c $(PARSE_DIR)parsing_utils.c \
$(PARSE_DIR)tokenize.c $(PARSE_DIR)interpret.c $(PARSE_DIR)red_utils.c \
$(EXEC_DIR)make_exec.c $(EXEC_DIR)run_command.c \
$(BUILTIN_DIR)builtin.c $(BUILTIN_DIR)cd.c $(BUILTIN_DIR)echo.c $(BUILTIN_DIR)env.c $(BUILTIN_DIR)exit.c \
$(BUILTIN_DIR)export_util.c $(BUILTIN_DIR)export_util2.c $(BUILTIN_DIR)export.c $(BUILTIN_DIR)pwd.c $(BUILTIN_DIR)unset.c \
$(NONBUILTIN_DIR)non_builtin.c \
$(REDIR_DIR)heredoc.c $(REDIR_DIR)redirect.c \
$(UTILS_DIR)ft_isalpha.c $(UTILS_DIR)ft_isdigit.c $(UTILS_DIR)ft_split.c $(UTILS_DIR)ft_strchr.c $(UTILS_DIR)ft_strcmp.c \
$(UTILS_DIR)ft_strdup.c $(UTILS_DIR)ft_strjoin.c $(UTILS_DIR)ft_strlen.c $(UTILS_DIR)ft_strncmp.c $(UTILS_DIR)utils.c \
$(UTILS_DIR)ft_itoa.c $(UTILS_DIR)ft_atoi.c $(UTILS_DIR)ft_putstr_fd.c  $(UTILS_DIR)error.c \
$(MAIN_DIR)free_main.c $(MAIN_DIR)free_redir.c

OBJS = $(SRCS:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) -I $(HEADER_DIR) -c $< -o $(<:.c=.o) 

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -I $(HEADER_DIR) $(OBJS) -lreadline -o $(NAME)

clean :
	rm -rf $(OBJS) $(BONUS_OBJS)

fclean : clean
	rm -rf $(NAME) $(BONUS_NAME)

re : fclean
	$(MAKE) all

.PHONY : all clean fclean re
