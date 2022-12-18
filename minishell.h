#ifndef MINISHELL_H
# define MINISHELL_H

// # include <readline/readline.h>
// # include <readline/history.h>
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
	t_node 	*head;
	t_node	*curr;
	char 	**envp;
	int		pipe_num;
	int		stdin_fd;
	int		stdout_fd;
} t_data;

int make_token(char *line, t_node *head);

/* run command */
int run_command(t_node *head, char **envp);
int	make_fork(t_data *data, int prev_fd);
void make_exec(t_data *data);

# endif