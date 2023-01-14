/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 02:51:49 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/14 22:26:02 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_type
{
	WORD,
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
	EXEC_ERROR,
	PERMISSION_ERROR,
}	t_error;

typedef struct s_envp_node
{
	char				*key;
	char				*value;
	int					idx;
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
	int						is_heredoc;
	char					*file;
	char					*limiter;
	struct s_infile_node	*hnext;
	struct s_infile_node	*next;
}	t_infile_node;

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
	char				**cmd;
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
	t_parsing_node	*parse;
	t_infile_node	*heredoc_node;
	t_cmd_node		*node_head;
	t_cmd_node		*curr;
}	t_main_node;

/* parse_cmd */
t_cmd_node	*new_cmd_node(t_parsing_node **parse, int i, t_main_node *main);
void		make_cmd_list(t_parsing_node *parse, t_main_node *main);
char		**set_cmd(t_parsing_node *parsing, t_main_node *main);
int			get_cmd_num(t_parsing_node *parsing, t_main_node *main);
void		init_cmd_node(t_cmd_node *node);

/* parse_envp */
t_envp_node	*parse_envp(char **ev);
t_envp_node	*new_envp_node(char *envp);
char		*ft_substr(char *s, int start, int len);
char		*get_value_by_key(t_envp_node *ev_lst, char *key, int len);

/* parse_red */
void		set_red(t_parsing_node *parse, t_cmd_node *node, t_main_node *main);
void		set_red_lst(t_parsing_node *parsing, t_cmd_node *node, \
t_main_node *main);
void		append_infile_node(t_infile_node *lst, void *red_node);
void		append_heredoc_node(t_infile_node *lst, void *red_node);
void		append_outfile_node(t_outfile_node *lst, void *red_node);
void		set_infile_node(t_parsing_node *parsing, void *node);
void		set_outfile_node(t_parsing_node *parsing, void *node);
void		*ft_malloc(int size);

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

/* interpret */
void		ft_interpret(t_parsing_node *parse, t_envp_node *ev_lst, \
int status);
char		*interpret(char *str, t_envp_node *ev_lst, int status);
char		*ft_strcat(char *str, char *value);
int			get_len_ev(char *str, t_envp_node *ev_lst, int status);

/* interpret_utils */
int			check_dollar(char *str);
int			key_to_value(char *str, t_envp_node *ev_lst,
				char **value, int status);
void		rm_quote(t_parsing_node *node);

/* red_utils */
void		append_infile_node(t_infile_node *lst, void *red_node);
void		append_heredoc_node(t_infile_node *lst, void *red_node);
void		append_outfile_node(t_outfile_node *lst, void *red_node);
void		*ft_calloc(size_t count, size_t size);
void		ft_bzero(void *s, size_t n);

/* exec */
void		restore_std(t_main_node *main);
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
int			ft_exit_atoi(char *str, int *is_not_num, int i);
void		add_env(t_main_node *main, char *key, char *value);
int			is_invalid_key(char *s, int flag);
char		*get_env_path(t_envp_node *envp, char *key);
char		**make_key_value(char *cmd);
int			ft_search_char(char *s, char c);
t_envp_node	*new_envp_node(char *envp);
void		swap_envp_node(t_envp_node *curr, t_envp_node *next);
void		restore_envp_order(t_main_node *main);
void		envp_ascending_order(t_main_node *main);
void		show_export(t_main_node *main);

/* utils */
void		ft_free_str(char **s1, char **s2);
size_t		ft_double_strlen(char **str);
char		*ft_strjoin3(char *s1, char *s2, char *s3);

/* error */
int			perror_comment(t_main_node *main, char *s1, char *s2, int status);
void		ft_putstr_err(char *cmd, char *arg, char *comment, t_error error);
int			error_msg(t_main_node *main, char *arg, t_error error, int status);

/* libft */
int			ft_atoi(const char *str);
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
void		*ft_calloc(size_t count, size_t size);
void		ft_putstr_fd(char *s, int fd);

/* envp */
char		*ft_strchr(const char *s, int c);
t_envp_node	*parse_envp(char **ev);

/* free */
void		ft_output_clear(t_outfile_node **lst);
void		ft_input_clear(t_infile_node **lst);
void		ft_node_clear(t_cmd_node **lst);
void		ft_envp_clear(t_envp_node **lst);
void		free_main(t_main_node *main);

void		cmd_node_clear(t_cmd_node	*nodes);
void		infile_node_clear(t_infile_node	*nodes);
void		outfile_node_clear(t_outfile_node	*nodes);
void		free_double_char(char **cmd);
void		free_one_node(t_cmd_node *node);
void		*ft_free(void *ptr);
void		parse_node_clear(t_parsing_node	*nodes);

/* syntax_err */
int			print_syntax_err(const char *str);
int			check_pipe_err(t_parsing_node *parse, t_parsing_node *now);
int			check_red_err(t_parsing_node *now);
int			check_quote_err(char *line);

/* signal */
void		set_signal(int sigint, int sigquit);
void		action(int signum);
void		rm_ctrl(void);

#endif
