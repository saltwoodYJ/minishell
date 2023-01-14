/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   run_command.c									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/07 00:12:12 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/13 21:14:02 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

int	no_cmd(t_main_node *main, int flag)
{
	if (flag == 0)
	{
		if (!(main->node_head->next) || !(main->node_head->next->cmd) || \
		!(main->node_head->next->cmd[0]))
		{
			input_redirect(main);
			output_redirect(main);
			return (1);
		}
	}
	else if (flag == 1)
	{
		if (!(main->curr->cmd) || !(main->curr->cmd[0]))
		{
			input_redirect(main);
			output_redirect(main);
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

void	run_waitpid(t_main_node *main, int pid, int i)
{
	int			signo;
	int			res;
	int			status;

	set_signal(0, 0);
	res = waitpid(-1, &status, 0);
	set_signal(2, 2);
	if (WIFSIGNALED(status))
	{
		signo = WTERMSIG(status);
		if (signo == SIGINT && i == 0)
			ft_putstr_fd("^C\n", STDERR_FILENO);
		else if (signo == SIGQUIT && i == 0)
			ft_putstr_fd("^\\Quit: 3\n", STDERR_FILENO);
		if (res == pid)
			main->status = 128 + signo;
	}
	else if (res == pid && WIFEXITED(status))
		main->status = WEXITSTATUS(status);
}

int	run_command(t_main_node *main)
{
	int			prev_fd;
	int			i;
	int			pid;

	i = -1;
	prev_fd = dup(0);
	if (only_one_builtin(main))
		return (0);
	while (++i < main->cmd_num - 1)
	{
		prev_fd = make_pipe(main, prev_fd);
		if (prev_fd == -1)
			return (1);
		main->curr = main->curr->next;
	}
	pid = exec_last(main, prev_fd);
	i = -1;
	while (++i < main->cmd_num)
		run_waitpid(main, pid, i);
	return (0);
}
