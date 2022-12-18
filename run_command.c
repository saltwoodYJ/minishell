
#include "minishell.h"

int	ft_exit(char *str, int code)
{
	perror(str);
	exit(code);
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

	path = get_path(data->envp, cmd[0]);
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
