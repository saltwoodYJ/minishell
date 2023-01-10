/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 02:51:49 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/10 21:16:02 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>

typedef enum e_type
{
	WORD,
	//REDIRECT (INPUT, OUTPUT, HEREDOC, ADD_OUTPUT)
	RED_I,
	RED_O,
	RED_A,
	RED_H,
	PIPE,
	T_NULL
}	t_type;

typedef enum s_error
{
	FILE_ERROR,
	CMD_ERROR,
	EXPORT_KEY_ERROR,
	UNSET_KEY_ERROR,
	NOT_SET_ERROR,
	NUM_ARG_ERROR,
	MANY_ARG_ERROR,
	EXEC_ERROR
} t_error;

typedef struct s_envp_node
{
	char				*key;
	char				*value;
	struct s_envp_node	*next;
}	t_envp_node;

typedef struct s_parsing_node
{
	char					*str;
	t_type					type;
	struct s_parsing_node	*next;
}	t_parsing_node;

/* hnext->heredoc, next-> infile
is_heredoc: 0(false), 1(true)*/
typedef struct s_infile_node
{
	int						is_heardoc;
	char					*file;
	char					*limiter;
	struct s_infile_node	*hnext;
	struct s_infile_node	*next;
}	t_infile_node;

// >,>>
/* type: 0, type: 1 APPEND */
typedef struct s_outfile_node
{
	char					*file;
	int						type;
	struct s_outfile_node	*next;
}	t_outfile_node;

typedef struct s_cmd_node
{
	int					idx;
	//cmd: "echo -n "helloworld"" 'echo', '-n', 'helloworld'
	char				**cmd;
	// < a <b <<A <<C <<D <a
	//infile_node: a b A C D a
	t_infile_node		*infile_node;
	t_outfile_node		*outfile_node;
	struct s_cmd_node	*next;
}	t_cmd_node;

typedef struct s_main_node
{
	t_envp_node		*ev_lst;
	char			**ev;
	int				stdin_fd;
	int				stdout_fd;
	int				cmd_num;
	int				status;
	// A C D 파이프 통틀어서 모든 히어독 노드들 저장
	t_infile_node	*heardoc_node;
	t_cmd_node		*node_head;
	t_cmd_node		*curr;
}	t_main_node;

/* parse_cmd */
t_cmd_node	*new_cmd_node(t_parsing_node **parse, int i, t_main_node *main);
void		make_cmd_list(t_parsing_node *parse, t_main_node *main);
char		**set_cmd(t_parsing_node *parsing);
int			get_cmd_num(t_parsing_node *parsing);
void		init_cmd_node(t_cmd_node *node);

/* parse_envp */
t_envp_node	*parse_envp(char **ev);
t_envp_node	*new_envp_node(char *envp);
char		*ft_substr(char *s, int start, int len);
t_envp_node	*get_value_by_key(t_envp_node *ev_lst, char *key);

/* parse_red */
void		set_red(t_parsing_node *parse, t_cmd_node *node, t_main_node *main);
void		set_red_lst(t_parsing_node *parsing, t_cmd_node *node, \
t_main_node *main);
void		append_infile_node(t_infile_node *lst, void *red_node);
void		append_heardoc_node(t_infile_node *lst, void *red_node);
void		append_outfile_node(t_outfile_node *lst, void *red_node);
void		set_infile_node(t_parsing_node *parsing, void *node);
void		set_outfile_node(t_parsing_node *parsing, void *node);
void		*new_red_node(int size);

/* parsing_utils */
void		make_sep(char *s, int *index, int *len);
int			is_sep(char *str, int index);
int			is_quote(char *s, int index);
int			get_type(char *str);

/* tokenize */
int			make_token(char *line, t_main_node *main);
char		*ft_worddup(char *str, int index, int len);
void		ft_parse(char *s, t_parsing_node **parse);
int			add_parsing_node(t_parsing_node **now, char *str);

/* exec */
int			no_cmd(t_main_node *main, int flag);
int			run_command(t_main_node *main);
int			make_pipe(t_main_node *main, int prev_fd);
void		make_exec(t_main_node *main, int flag);
int			exec_last(t_main_node *main, int prev_fd);

/*redirect*/
int			read_line(char *limiter, int infile);
char		*make_heredoc(char *limiter, int idx);
int			input_redirect(t_main_node *main);
int			output_redirect(t_main_node *main);
void		clear_heredoc(t_main_node *main);
void		set_heredoc(t_main_node *main);

/*non builtin*/
void		exec_non_builtin(t_main_node *main);
char		**search_origin_path(t_envp_node *envp);
char		*get_path(char *first_cmd, char **splited_path);

/* builtin */
int			check_builtin(char *str);
void		exec_builtin(t_main_node *main);
void		ft_echo(t_main_node *main);
void		ft_cd(t_main_node *main);
void		ft_pwd(t_main_node *main);
void		ft_export(t_main_node *main);
void		ft_unset(t_main_node *main);
void		ft_env(t_main_node *main);
void		ft_exit(t_main_node *main);

/*builtin utils*/
int			ft_exit_atoi(const char *str, int *is_char, int i);
void		add_env(t_main_node *main, char *key, char *value);
int			is_invalid_key(char *s, int flag);
char		*get_env_path(t_envp_node *envp, char *key);
char		**make_key_value(char *cmd);

/* utils */
void		ft_free(char **s1, char **s2);
size_t		ft_double_strlen(char **str);
void		perror_comment(char *s1, char *s2);
char		*ft_strjoin3(char *s1, char *s2, char *s3);
int			error_msg(t_main_node *main, char *arg, t_error error, int status);

/* libft */
char		*ft_strchr(const char *s, int c);
char		*ft_strjoin(char const *s1, char const *s2);
int			ft_strlen(char *str);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(char *s1, char *s2);
char		**ft_split(char const *s, char c);
int			ft_isdigit(int c);
int			ft_isalpha(int c);
char		*ft_strdup(const char *s1);
char		*ft_substr(char *s, int start, int len);
char		*ft_itoa(int n);

/* envp */
char		*ft_strchr(const char *s, int c);
t_envp_node	*parse_envp(char **ev);
t_envp_node	*new_envp_node(char *envp);
t_envp_node	*get_value_by_key(t_envp_node *ev_lst, char *key);
int			search_equal(char *s);

/* free */
void		ft_output_clear(t_outfile_node **lst);
void		ft_input_clear(t_infile_node **lst);
void		ft_node_clear(t_cmd_node **lst);
void		ft_envp_clear(t_envp_node **lst);
void		free_main(t_main_node *main);

#endif
