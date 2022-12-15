#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>

typedef enum e_type
{
    WORD,
    REDIRECT,
    PIPE,
    T_NULL
} t_type;

typedef struct s_node
{
    char    *str;
    int     type;
    t_node  *next;
} t_node;

int make_token(char *line, t_node *head);
int run_command(t_node *head);

# endif