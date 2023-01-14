/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   builtin.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/06 23:38:35 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/13 15:34:05 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

int	check_builtin(char *str)
{
	if (!ft_strcmp(str, "echo") || !ft_strcmp(str, "cd") || \
		!ft_strcmp(str, "pwd") || !ft_strcmp(str, "export") || \
		!ft_strcmp(str, "unset") || !ft_strcmp(str, "env") || \
		!ft_strcmp(str, "exit"))
		return (1);
	return (0);
}

void	exec_builtin(t_main_node *main)
{
	t_cmd_node	*curr;

	curr = main->curr;
	if (ft_strcmp(curr->cmd[0], "echo") == 0)
		ft_echo(main);
	else if (ft_strcmp(curr->cmd[0], "cd") == 0)
		ft_cd(main);
	else if (ft_strcmp(curr->cmd[0], "pwd") == 0)
		ft_pwd(main);
	else if (ft_strcmp(curr->cmd[0], "export") == 0)
		ft_export(main);
	else if (ft_strcmp(curr->cmd[0], "unset") == 0)
		ft_unset(main);
	else if (ft_strcmp(curr->cmd[0], "env") == 0)
		ft_env(main);
	else if (ft_strcmp(curr->cmd[0], "exit") == 0)
		ft_exit(main);
}
