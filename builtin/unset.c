/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   unset.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/06 23:38:26 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/10 16:19:50 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

int	remove_env(t_main_node *main, char *key)
{
	t_envp_node	*pre;
	t_envp_node	*old;

	if (is_invalid_key(key, 1))
		return (error_msg(main, key, UNSET_KEY_ERROR, 1));
	pre = main->ev_lst->next;
	while (pre->next != NULL)
	{
		if (!ft_strcmp(pre->next->key, key))
		{
			old = pre->next;
			if (old == NULL)
				return (0);
			pre->next = old->next;
			free(old->key);
			free(old->value);
			free(old);
			return (0);
		}
		else
			pre = pre->next;
	}
	return (0);
}

void	ft_unset(t_main_node *main)
{
	char		**cmd;
	int			i;
	int			res;

	res = 0;
	cmd = main->curr->cmd;
	i = 1;
	while (cmd[i])
	{
		res += remove_env(main, cmd[i]);
		i++;
	}
	if (res > 0)
		main->status = 1;
	else
		main->status = 0;
}
