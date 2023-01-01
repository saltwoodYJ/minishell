
#include "minishell.h"

int check_builtin(char *str)
{
	if (!ft_strcmp(str, "echo") || !ft_strcmp(str, "cd") || \
		!ft_strcmp(str, "pwd") || !ft_strcmp(str, "export") || \
		!ft_strcmp(str, "unset") || !ft_strcmp(str, "env") || \
		!ft_strcmp(str, "exit") || !ft_strcmp(str, "$?"))
		return (1);
	return (0);
}

void make_exec(t_main_node *main, int flag)
{
	t_cmd_node *curr;

	input_redirect(main_node);
	output_redirect(main_node);
	if (check_builtin(main->node_head->cmd[0]))
		exec_builtin(main_node);
	else
		exec_non_builtin(main_node);
	if (flag != -1)
		exit(0);
}

int	make_pipe(t_main_node *main, int prev_fd)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

	if (pipe(fd) == -1)
		printf("pipe_error");
	pid = fork();
	if (pid < 0)
		printf("fork_error");
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
		make_exec(main_node, 0);
	}
    waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		main->status = WEXITSTATUS(status); //공부좀해
	return (prev_fd);
}

int	last_command(t_main_node *main, int prev_fd)
{
	pid_t	pid;
	int		status = 0;

	pid = fork();
	if (pid < 0)
		printf("fork_error");
	if (pid > 0) //부모
		close(prev_fd); //여기서 안쓰니까 닫기
	if (pid == 0) //자식
	{
		dup2(prev_fd, 0); //stdin으로 던져주기
		close(prev_fd); //썼으니까 닫아주기 (파일은 더이상 안쓰니까)
		make_exec(main_node, 0);
	}
    waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		main->status = WEXITSTATUS(status); //공부좀해
	return (0);
}

int run_command(t_main_node *main)
{
	int		prev_fd;
	int		i;
	int		status;

	i = -1;
	prev_fd = dup(0);
	main->node_head = main->node_head->next;
	while (++i < main->cmd_num - 1)
	{
		prev_fd = make_pipe(main_node, prev_fd);
		main->node_head = main->node_head->next;
	}
	i = -1;
	while (++i < main->cmd_num - 1)
		waitpid(0, &status, 0);
	if (main->cmd_num == 1 && check_builtin(main->node_head->cmd[0]))
		make_exec(main_node, -1);
	else
	{
		last_command(main_node, prev_fd);
		waitpid(0, &status, 0);
	}
	return (0);
}