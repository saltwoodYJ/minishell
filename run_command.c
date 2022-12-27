
#include "minishell.h"

int check_builtin(char *str)
{
	if (!ft_strncmp(str, "echo", 5) || !ft_strncmp(str, "cd", 3) || \
		!ft_strncmp(str, "pwd", 4) || !ft_strncmp(str, "export", 7) || \
		!ft_strncmp(str, "unset", 6) || !ft_strncmp(str, "env", 4) || \
		!ft_strncmp(str, "exit", 5) || !ft_strncmp(str, "$?", 3))
		return (1);
	return (0);
}

void make_exec(t_cmd *cmd)
{
	t_node *curr;

    curr = cmd->head->next;
	input_redirect(cmd);
	output_redirect(cmd);
	if (curr->type == WORD)
	{
		if (check_builtin(curr->str))
			exec_builtin(cmd);
		else
			exec_non_builtin(cmd);
	}
}

int	make_pipe(t_cmd *cmd, int prev_fd)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

	if (pipe(fd) == -1)
		ft_exit("pipe_error", 1);
	pid = fork();
	if (pid < 0)
		ft_exit("fork_error", 1);
	if (pid > 0) //부모
	{
		close(fd[1]);
		close(prev_fd); //여기서 안쓰니까 닫기
		prev_fd = dup(fd[0]); //리턴해줄 prev_fd
	}
	if (pid == 0) //자식
	{
		dup2(prev_fd, 0); //stdin으로 던져주기
		close(prev_fd); //썼으니까 닫아주기 (파일은 더이상 안쓰니까)
		dup2(fd[1], 1); //
		close(fd[1]);
		make_exec(cmd);
		dup2(cmd->info->stdout_fd, 1);
		printf("execve error!\n");
		if(errno == ENOEXEC) //errno 처리
            exit(126);
        else if(errno == ENOENT) //errno 처리
            exit(127);
        else
            exit(EXIT_FAILURE);
	}
    // waitpid(pid, &status, 0);
	// cmd->info->status = status; //공부좀해
	return (prev_fd);
}

int run_command(t_data *data, char **envp)
{
	int		prev_fd;
	int		i;
	int		status;

	i = 0;
	prev_fd = dup(0);
	while (i < data->pipe_num)
	{
		data->cmd[i].info = &info;
		prev_fd = make_pipe(&(data->cmd[i]), prev_fd);
		i++;
	}
	i = 0;
	while (i < data->pipe_num)
	{
		waitpid(0, &status, WNOHANG);
		i++;
	}
	if (data->pipe_num == 0 && check_builtin(data->cmd[i].head->next->str))
		make_exec(&(data->cmd[i]));
	else
		make_pipe(&(data->cmd[i]), prev_fd);
	return (0);
}
