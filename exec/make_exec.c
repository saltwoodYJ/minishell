/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:12:06 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/09 17:42:14 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_exec(t_main_node *main, int flag)
{
	if (input_redirect(main) || output_redirect(main))
	{
		clear_heredoc(main);
		main->status = 1;
		if (flag == -1)
			return ;
		else
			exit(1);
	}
	else
		clear_heredoc(main);
	if (check_builtin(main->curr->cmd[0]))
		exec_builtin(main);
	else
		exec_non_builtin(main); //알아서 exit함
	if (flag != -1)
		exit(main->status); //>>builtin이고 명령어가 하나<<가 아니었을때.
}

int run_process(t_main_node *main, pid_t pid, int fd[2], int prev_fd)
{
	if (pid > 0)
	{
		close(fd[1]);
		close(prev_fd); //여기서 안쓰니까 닫기
		prev_fd = dup(fd[0]); //리턴해줄 prev_fd
	}
	if (pid == 0)
	{
		dup2(prev_fd, 0); //stdin으로 던져주기
		dup2(fd[1], 1);
		close(prev_fd); //썼으니까 닫아주기 (파일은 더이상 안쓰니까)
		close(fd[1]);
		make_exec(main, 0);
	}
	return (prev_fd);
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
	prev_fd = run_process(main, pid, fd, prev_fd);
	return (prev_fd);
}

int	exec_last(t_main_node *main, int prev_fd)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		printf("fork_error");
		return (-1);
	}
	if (pid > 0)
		close(prev_fd);
	if (pid == 0)
	{
		dup2(prev_fd, 0);
		close(prev_fd);
		make_exec(main, 0);
	}
	return (0);
}
