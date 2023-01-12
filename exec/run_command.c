/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:12:12 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/11 15:11:38 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	no_cmd(t_main_node *main, int flag)
{
	if (flag == 0)
	{
		if (!(main->node_head->next) || !(main->node_head->next->cmd) || !(main->node_head->next->cmd[0]))
		{
			input_redirect(main);
			output_redirect(main);
			clear_heredoc(main);
			return (1);
		}
	}
	else if (flag == 1)
	{
		if (!(main->curr->cmd) || !(main->curr->cmd[0]))
		{
			input_redirect(main);
			output_redirect(main);
			clear_heredoc(main);
			return (1);
		}
	}
	return (0);
}

int	only_one_builtin(t_main_node *main)
{
	if (main->cmd_num == 1 && check_builtin(main->curr->cmd[0]))
	{
		make_exec(main, 1);
		return (1);
	}
	return (0);
}

int	run_command(t_main_node *main)
{
	int		prev_fd;
	int		i;
	int		status;
	pid_t	pid;
	t_cmd_node	*tmp;

	i = -1;
	prev_fd = dup(0);
	if (no_cmd(main, 0) || only_one_builtin(main))
		return (0);
	while (++i < main->cmd_num - 1)
	{
		prev_fd = make_pipe(main, prev_fd);
		if (prev_fd == -1)
			return (1);
		tmp = main->curr;
		main->curr = main->curr->next;
//		cmd_node_clear(tmp);
		printf("clear!");
	}
	pid = exec_last(main, prev_fd);
	i = -1;
	while (++i < main->cmd_num)
	{
		if (waitpid(-1, &status, 0) == pid && (WIFEXITED(status)))
			main->status = WEXITSTATUS(status);
	}
	return (0);
}
