/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:12:12 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/09 17:47:23 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_under_one(t_main_node *main)
{
	if (!(main->node_head->next->cmd[0]))
	{
		if (input_redirect(main) || output_redirect(main))
			main->status = 1;
		clear_heredoc(main);
		return (1);
	}
	else if (main->cmd_num == 1 && check_builtin(main->curr->cmd[0]))
	{
		make_exec(main, -1); //빌트인이면서 하나의 명령어일경우 (exit을 실행하지 않는다. fork도 쓰지 않는다)
		return (1);
	}
	return (0);
}

int	run_command(t_main_node *main)
{
	int		prev_fd;
	int		i;
	int		status;

	i = -1;
	prev_fd = dup(0);
	if (cmd_under_one(main)) //1이라는건 하나의 명령어라는 뜻
		return (0);
	while (++i < main->cmd_num - 1)
	{
		prev_fd = make_pipe(main, prev_fd);
		if (prev_fd == -1)
		{
			main->status = 1;
			return (1);
		}
		main->curr = main->curr->next;
	}
	exec_last(main, prev_fd); //마지막 명령어에선 파이프 쓰지 않는다
	i = -1;
	while (++i < main->cmd_num)
	{
		waitpid(-1, &status, 0); //마지막으로 종료된 pid. sleep 1 | sleep 3
		if (WIFEXITED(status))
			main->status = WEXITSTATUS(status);
	}
	return (0);
}
