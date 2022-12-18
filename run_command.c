
#include "minishell.h"

int	ft_exit(char *str, int code)
{
	perror(str);
	exit(code);
}

char	**search_cmd(t_data* data)
{
	char	**cmd;
	t_node	*temp;
	int		idx;

	temp = data->curr;
	idx = 0;
	while (temp != NULL && temp->type == WORD)
	{
		idx++;
		temp = temp->next;
	}
	cmd = (char **)malloc(sizeof(char *) * idx + 1);
	idx = 0;
	while (data->curr != NULL &&  data->curr->type == WORD)
	{
		cmd[idx] =  data->curr->str;
		idx++;
		data->curr =  data->curr->next;
	}
	cmd[idx] = 0;
	return (cmd);
}

char	**search_origin_path(char **envp)
{
	int		i;
	char	*origin_path;

	i = 0;
	while (envp[i] != 0)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			origin_path = envp[i] + 5;
			return (ft_split(origin_path, ':'));
		}
		i++;
	}
	return (0);
}

char	*get_path(char **envp, char *first_cmd, char *path)
{
	int		i;
	int		path_count;
	char	*temp_path;
	char	**splited_path;
	char	*cmd_path;

	if (access(first_cmd, X_OK) == 0)
		return (first_cmd);
	splited_path = search_origin_path(envp);
	path_count = ft_double_strlen(splited_path);
	temp_path = ft_strjoin("/", first_cmd, 0, 0);
	i = 0;
	while (i < path_count)
	{
		cmd_path = ft_strjoin(splited_path[i], temp_path, 0, 0);
		if (access(cmd_path, X_OK) == 0)
			break ;
		else
			ft_free(0, &cmd_path);
		i++;
	}
	ft_free(splited_path, 0);
	ft_free(0, &temp_path);
	return (cmd_path);
}

void	ft_putstr_err(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return ;
	i = 0;
	while (s1[i] != 0)
	{
		write(2, &s1[i], 1);
		i++;
	}
	i = 0;
	while (s2[i] != 0)
	{
		write(2, &s2[i], 1);
		i++;
	}
	write(2, "\n", 1);
}

int make_output(t_data *data, int output)
{
	t_node *temp;
	int		i;
	int		fd;

	i = 0;
	temp = data->curr;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->str, ">", 1))
			break;
		temp = temp -> next; 
	}
	/* 파일 오픈 */
	fd = open(temp->str, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		printf("fd error!!\n");
		exit(0);
	}
	return (fd);
}

int make_input(t_data *data, int input)
{
	t_node *temp;
	int		i;
	int		fd;

	i = 0;
	temp = data->curr;
	while (temp != NULL && i != input)
	{
		if (!ft_strncmp(temp->str, "<", 1))
			i++;
		temp = temp -> next; 
	}
	/* 파일 오픈 */
	fd = open(temp->str, O_RDONLY);
	if (fd < 0)
	{
		printf("fd error!!\n");
		exit(0);
	}
	return (fd);
}

void	input_redirect(t_data *data)
{
	t_node *temp;
	int		input_fd;

	temp = data->curr;
	while (temp != NULL && temp->type != PIPE)
	{
		if (temp->type == REDIRECT && ft_strncmp(temp->str, "<", 1) == 0)
		{
			temp = temp -> next;
			input_fd = open(temp->str, O_RDONLY);
			if (input_fd < 0)
			{
				dup2(data->stdin_fd, 0);
				printf("No such file or directory");
				exit(0);
			}
			else
			{
				dup2(input_fd, 0);
				// close(prev_input);
				// prev_input = input_fd;
			}
		}
		temp = temp-> next;
	}
}

void	output_redirect(t_data *data)
{
	t_node *temp;
	int		output_fd;

	temp = data->curr;
	while (temp != NULL && temp->type != PIPE)
	{
		if (temp->type == REDIRECT && ft_strncmp(temp->str, ">", 1) == 0)
		{
			temp = temp -> next;
			output_fd = open(temp->str, O_RDWR | O_CREAT | O_TRUNC, 0644);
			if (output_fd < 0)
			{
				dup2(data->stdout_fd, 1);
				printf("No such file or directory");
				exit(0);
			}
			else
				dup2(output_fd, 1);
		}
		temp = temp-> next;
	}
}

void make_exec(t_data *data)
{
	char 	**cmd;
	char	*path;
	int		input;
	int		output;

	/* cmd 합치기 */
	cmd = search_cmd(data);
	if (!cmd)
		return ; //오류 처리

	path = get_path(data->envp, cmd[0], path);
	if (!path)
	{
		ft_putstr_err(cmd[0], ": command not found");
		ft_free(cmd, 0);
		exit(127);
	}
	execve(path, cmd, data->envp);
	write(2, "execve error\n", 13);
	exit(0);
}

int	make_fork(t_data *data, int prev_fd)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		ft_exit("pipe_error", 1);
	pid = fork();
	if (pid < 0)
		ft_exit("fork_error", 1);
	if (pid > 0) //부모
	{
		close(fd[1]);
		close(prev_fd);
		prev_fd = dup(fd[0]);
	}
	if (pid == 0) //자식
	{
		dup2(prev_fd, 0);
		close(prev_fd);
		dup2(fd[1], 1);
		input_redirect(data);
		output_redirect(data);
		close(fd[1]);
		make_exec(data);
	}
	return (prev_fd);
}

int count_pipe(t_node *head)
{
	t_node *curr;
	int pipe_num;

	curr = head -> next;
	pipe_num = 0;
	while (curr != NULL)
	{
		if (curr->type == PIPE)
			pipe_num++;
		curr = curr -> next;
	}
	return (pipe_num);
}

int run_command(t_node *head, char **envp)
{
	t_data 	data;
	int		pipe_num;
	int		fd;
	int		input;
	int		output;
	int		prev_fd;
	int		i;

	data.head = head;
	data.curr = head->next;
	data.envp = envp;
	if (!data.curr)
	{
		printf("data error!\n");
		exit(0);
	}
	pipe_num = count_pipe(head);
	prev_fd = dup(0);
	data.stdin_fd = dup(0);
	data.stdout_fd = dup(1);
	i = 0;
	while (i < pipe_num)
	{
		/* 리다이렉션이 있으면?*/
		/* 살행 가능한 빌트인 함수인지 확인 */
		prev_fd = make_fork(&data, prev_fd);
		while (data.curr != NULL && data.curr->type != PIPE)
		{
			if (data.curr->type == PIPE)
				break;
			data.curr = data.curr->next;
		}
		data.curr = data.curr->next;
		i++;
	}
	i = 0;
	while (i < pipe_num)
	{
		waitpid(0, 0, WNOHANG);
		i++;
	}
	input_redirect(&data);
	output_redirect(&data);
	make_exec(&data);
	return (0);
}
