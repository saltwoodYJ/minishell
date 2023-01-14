/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   env.c											  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/06 23:38:28 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/13 16:56:04 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_main_node *main)
{
	t_envp_node	*curr;

	if (main->curr->cmd[1])
	{
		error_msg(main, main->curr->cmd[1], FILE_ERROR, 127);
		return ;
	}
	curr = main->ev_lst->next;
	while (curr != NULL)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	main->status = 0;
}
