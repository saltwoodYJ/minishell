#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>

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

typedef struct s_info
{
	int			stdin_fd;
	int			stdout_fd;
	char 		**envp;
} t_info;

typedef struct s_cmd
{
	t_node 		*head; //1개의 cmd를 담은 링크드 리스트
	// t_node		*curr;
	t_info		*info;
} t_cmd;

typedef struct s_data
{
	int     pipe_num; //pipe 개수
	t_cmd 	*cmd;
} t_data;

int make_token(char *line, t_node *head);

/* run command */
int run_command(t_data *data, char **envp);
int	make_pipe(t_cmd *cmd, int prev_fd);
void make_exec(t_cmd *cmd);

/*redirect*/
int		read_line(char *limiter, int infile, int len);
int		make_here_doc(char *limiter);
void	exec_here_doc(t_node *limiter);
void	input_redirect(t_cmd *cmd);
void	output_redirect(t_cmd *cmd);

/*non builtin*/
void	 exec_non_builtin(t_cmd *cmd);
char	**search_cmd(t_cmd *cmd);
char	**search_origin_path(char **envp);
char	*get_path(char **envp, char *first_cmd);

/* builtin */
void 	exec_builtin(t_cmd *cmd);
void 	ft_echo(t_cmd *cmd);
void 	ft_cd(t_cmd *cmd);
void 	ft_pwd();
void 	ft_export(t_cmd *cmd);
void 	ft_unset(t_cmd *cmd);
void 	ft_env(t_cmd *cmd);
void 	ft_exit2(t_cmd *cmd);

/* pipex */
void	ft_free(char **s1, char **s2);
char	*ft_strjoin(char const *s1, char const *s2, int i, int j);
size_t	ft_strlen(char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_double_strlen(char **str);
char	**ft_split(char const *s, char c);

/* error */
void	ft_putstr_err(char *s1, char *s2);
int		ft_exit(char *str, int code);

# endif