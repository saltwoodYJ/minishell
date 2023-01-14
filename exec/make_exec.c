/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:12:06 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/14 19:36:41 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_exec(t_main_node *main, int flag)
{
	if (input_redirect(main) || output_redirect(main))
	{
		if (flag == 1)
			return ;
		else
			exit(1);
	}
	if (check_builtin(main->curr->cmd[0]))
		exec_builtin(main);
	else
		exec_non_builtin(main);
	if (flag != 1)
		exit(main->status);
}

int	run_process(t_main_node *main, pid_t pid, int fd[2], int prev_fd)
{
	set_signal(2, 2);
	if (pid > 0)
	{
		close(fd[1]);
		close(prev_fd);
		prev_fd = dup(fd[0]);
		close(fd[0]);
	}
	if (pid == 0)
	{
		dup2(prev_fd, 0);
		dup2(fd[1], 1);
		close(prev_fd);
		close(fd[1]);
		close(fd[0]);
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
		error_msg(main, "pipe", EXEC_ERROR, 1);
		return (-1);
	}
	set_signal(1, 1);
	pid = fork();
	if (pid < 0)
	{
		error_msg(main, "fork", EXEC_ERROR, 1);
		return (-1);
	}
	prev_fd = run_process(main, pid, fd, prev_fd);
	return (prev_fd);
}

pid_t	exec_last(t_main_node *main, int prev_fd)
{
	pid_t	pid;
	int		status;

	status = 0;
	set_signal(1, 1);
	pid = fork();
	if (pid < 0)
	{
		error_msg(main, "fork", EXEC_ERROR, 1);
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
	return (pid);
}
