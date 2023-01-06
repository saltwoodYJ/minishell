
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
	input_redirect(main);
	output_redirect(main);
	if (check_builtin(main->curr->cmd[0]))
		exec_builtin(main);
	else
		exec_non_builtin(main); //알아서 exit함
	if (flag != -1) 
		exit(main->status); //>>builtin이고 명령어가 하나<<가 아니었을때.
}

int	make_pipe(t_main_node *main, int prev_fd)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		printf("pipe_error");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		printf("fork_error");
		exit(1);
	}
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
		dup2(fd[1], 1); 
		close(fd[1]);
		make_exec(main, 0);
	}
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
		make_exec(main, 0);
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
	main->curr = main->node_head->next;
	while (++i < main->cmd_num - 1)
	{
		prev_fd = make_pipe(main, prev_fd);
		main->curr = main->curr->next;
	}
	i = -1;
	while (++i < main->cmd_num - 1) //fork를 떴어야 여기로 넘어옴
	{
		waitpid(-1, &status, 0); //마지막으로 종료된 pid. sleep 1 | sleep 3 (4초를 기다리게 됨. 3초만 기다리게 함)
		if (WIFEXITED(status))
			main->status = WEXITSTATUS(status); //공부좀해
	}
	if (main->cmd_num == 1 && check_builtin(main->curr->cmd[0]))
		make_exec(main, -1); //빌트인이면서 하나의 명령어일경우 (exit을 실행하지 않는다)
	else
	{
		last_command(main, prev_fd);
		waitpid(0, &status, 0);
	}
	return (0);
}