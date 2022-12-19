
#include "minishell.h"

// void whereis_cmd()
// {

// }

void make_exec(t_data *data)
{
	t_node *temp;

    temp = data->curr;
	input_redirect(data);
	output_redirect(data);
	if (temp->type == WORD)
	{
		if (!ft_strncmp(temp->str, "echo", 5) || !ft_strncmp(temp->str, "cd", 3) || \
		!ft_strncmp(temp->str, "pwd", 4) || !ft_strncmp(temp->str, "export", 7) || \
		!ft_strncmp(temp->str, "unset", 6) || !ft_strncmp(temp->str, "env", 4) || \
		!ft_strncmp(temp->str, "exit", 5))
			exec_builtin(data);
		else
			exec_nonbuiltin(data);
	}
}

int	make_pipe(t_data *data, int prev_fd)
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
	data.stdin_fd = dup(0);
	data.stdout_fd = dup(1);
	i = 0;
	while (i < pipe_num)
	{
		prev_fd = make_pipe(&data, prev_fd);
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
	make_exec(&data);
	return (0);
}
