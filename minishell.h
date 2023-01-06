#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
#include <errno.h>

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
} t_type;

 typedef struct s_envp_node
 {
	char *key;
	char *value;
	struct s_envp_node *next;
 } t_envp_node;
 
typedef	struct s_parsing_node
{
	char			*str;
	t_type			type;
	struct s_parsing_node	*next;
}	t_parsing_node;

typedef struct s_infile_node
{
	int						is_heardoc; // 0이면 infile 1이면 heredoc
	char					*file;
	char					*limiter;
	struct s_infile_node	*hnext; //heredoc
	struct s_infile_node	*next; //그냥 infile
}	t_infile_node;

// >,>>
typedef struct s_outfile_node
{
	char					*file;
	int						type; //0이면 덮어씌우기 1이면 append
	struct s_outfile_node	*next;
}	t_outfile_node;

typedef struct s_cmd_node
{
	int				idx;
	char			**cmd;// "echo -n "helloworld"" 'echo', '-n', 'helloworld'
//	char			*cmd_path[2]; 
// < a <b <<A <<C <<D <a
	t_infile_node	*heardoc_node; // A C D 파이프 통틀어서 모든 히어독 노드들 저장
	t_infile_node	*infile_node; // a b A C D a
	t_outfile_node	*outfile_node;
	struct s_cmd_node	*next;
}	t_cmd_node;

typedef struct s_main_node
{
	t_envp_node	*ev_lst;
	char		**ev;
	//char	**path; //
	int		input_fd;
	int   output_fd;
	int		cmd_num;
	int 	status;
	t_cmd_node	*node_head;
	t_cmd_node	*curr;
}	t_main_node;

/* parsing */
int make_token(char *line, t_main_node *main);
t_cmd_node	*new_node(char *str);
void	ft_parse(char *s, t_parsing_node **parse);
int	add_parsing_node(t_parsing_node **now, char *str);
int is_sep(char *str, int index);
int	is_quote(char *s, int index);
void	make_sep(char *s, int *index,int *len);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	get_type(char *str);

void	make_cmd_list(t_parsing_node *parse, t_main_node *main);
t_cmd_node	*make_cmd_node(t_parsing_node **p_now, t_cmd_node **c_now, int i);
char    **make_cmd(t_parsing_node *parsing);
int get_cmd_num(t_parsing_node *parsing);

void    init_cmd_node(t_cmd_node *node);

/* run command */
int run_command(t_main_node *main);
int	make_pipe(t_main_node *main, int prev_fd);
void make_exec(t_main_node *main, int flag);

/*redirect*/
int		read_line(char *limiter, int infile);
char	*make_heredoc(char *limiter);
void	input_redirect(t_main_node *main);
void	output_redirect(t_main_node *main);
void    set_heredoc(t_main_node *main);

/*non builtin*/
void	 exec_non_builtin(t_main_node *main);
char	**search_cmd(t_main_node *main);
char	**search_origin_path(t_envp_node *envp);
char	*get_path(t_envp_node *envp, char *first_cmd);

/* builtin */
int 	check_builtin(char *str);
void 	exec_builtin(t_main_node *main);
void 	ft_echo(t_main_node *main);
void 	ft_cd(t_main_node *main);
void 	ft_pwd(t_main_node *main);
void 	ft_export(t_main_node *main);
void 	ft_unset(t_main_node *main);
void 	ft_env(t_main_node *main);
void 	ft_exit(t_main_node *main);
int		ft_exit_atoi(const char *str, int *is_char);
void	add_env(t_main_node *main, char *key, char *value);
int 	is_valid_key(char *s);

/* pipex */
void	ft_free(char **s1, char **s2);
size_t	ft_double_strlen(char **str);

/* error */
void	ft_putstr_err(char *s1, char *s2);

/* libft */
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin3(char *s1, char *s2, char *s3);
size_t	ft_strlen(char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(char *s1, char *s2);
char	**ft_split(char const *s, char c);
int		ft_isdigit(int c);
char	*ft_strdup(const char *s1);

/* envp */
char	*ft_strchr(const char *s, int c);
t_envp_node	*parse_envp(char **ev);
t_envp_node	*new_envp_node(char *envp);
char	*ft_substr(char *s, int start, int len);
t_envp_node	*get_value_by_key(t_envp_node *ev_lst, char *key);

# endif