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
	char	**ev;
	//char	**path; //
	int		input_fd;
	int   output_fd;
	int		cmd_num;
	int 	status;
	t_cmd_node	*node_head;
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
int run_command(t_main_node *main_node);
int	make_pipe(t_main_node *main_node, int prev_fd);
void make_exec(t_main_node *main_node, int flag);

/*redirect*/
int		read_line(char *limiter, int infile, int len);
int		make_here_doc(char *limiter);
void	exec_here_doc(t_cmd_node *limiter);
// void	input_redirect(t_main_node *main_node);
// void	output_redirect(t_main_node *main_node);

/*non builtin*/
void	 exec_non_builtin(t_main_node *main_node);
char	**search_cmd(t_main_node *main_node);
char	**search_origin_path(char **envp);
char	*get_path(char **envp, char *first_cmd);

/* builtin */
int 	check_builtin(char *str);
void 	exec_builtin(t_main_node *main_node);
void 	ft_echo(t_main_node *main_node);
void 	ft_cd(t_main_node *main_node);
void 	ft_pwd();
void 	ft_export(t_main_node *main_node);
void 	ft_unset(t_main_node *main_node);
void 	ft_env(t_main_node *main_node);
void 	ft_exit(t_main_node *main_node);

/* pipex */
void	ft_free(char **s1, char **s2);
char	*ft_strjoin(char const *s1, char const *s2, int i, int j);
size_t	ft_strlen(char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(char *s1, char *s2);
size_t	ft_double_strlen(char **str);
char	**ft_split(char const *s, char c);
int		ft_atoi(const char *str);

/* error */
void	ft_putstr_err(char *s1, char *s2);

# endif