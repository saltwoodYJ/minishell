
#include "minishell.h"

// void whereis_cmd()
// {

// }

void make_exec(t_cmd *cmd)
{
	t_node *curr;

    curr = cmd->head->next;
	input_redirect(cmd);
	output_redirect(cmd);
	if (curr->type == WORD)
	{
		if (!ft_strncmp(curr->str, "echo", 5) || !ft_strncmp(curr->str, "cd", 3) || \
		!ft_strncmp(curr->str, "pwd", 4) || !ft_strncmp(curr->str, "export", 7) || \
		!ft_strncmp(curr->str, "unset", 6) || !ft_strncmp(curr->str, "env", 4) || \
		!ft_strncmp(curr->str, "exit", 5))
			exec_builtin(cmd);
		else
			exec_non_builtin(cmd);
	}
}

int	make_pipe(t_cmd *cmd, int prev_fd)
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
		make_exec(cmd);
	}
	return (prev_fd);
}

// int count_pipe(t_node *head)
// {
// 	t_node *curr;
// 	int pipe_num;

// 	curr = head -> next;
// 	pipe_num = 0;
// 	while (curr != NULL)
// 	{
// 		if (curr->type == PIPE)
// 			pipe_num++;
// 		curr = curr -> next;
// 	}
// 	return (pipe_num);
// }

int run_command(t_data *data, char **envp)
{
	t_info	info;
	int		prev_fd;
	int		i;

	i = 0;
	data->pipe_num = 0;
	info.stdin_fd = dup(0);
	info.stdout_fd = dup(1);
	info.envp = envp;
	while (i < data->pipe_num)
	{
		data->cmd[i].info = &info;
		// data->cmd[i] .curr = data->cmd[i].head->next;
		prev_fd = make_pipe(&(data->cmd[i]), prev_fd);
		i++;
	}
	i = 0;
	while (i < data->pipe_num)
	{
		waitpid(0, 0, WNOHANG);
		i++;
	}
	data->cmd[i].info = &info;
	make_exec(&(data->cmd[i]));
	return (0);
}
