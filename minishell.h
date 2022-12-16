#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include "pipex.h"

typedef enum e_type
{
	WORD,
	REDIRECT,
	PIPE,
	T_NULL
} t_type;

typedef struct s_node
{
	char           *str;
	int            type;
	struct s_node  *next;
} t_node;

typedef struct s_data
{
	char 	**envp;
	t_node 	*head;
	t_node	*curr;
	int		pipe_num;
} t_data;

int make_token(char *line, t_node *head);

/* run command */
int run_command(t_node *head, char **envp);
int	make_fork(t_data *data);
void make_exec(t_data *data);

# endif