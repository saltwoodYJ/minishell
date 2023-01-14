/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   export_util2.c									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/13 17:13:30 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/13 17:15:04 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

void	swap_envp_node(t_envp_node *curr, t_envp_node *next)
{
	char	*temp_key;
	char	*temp_value;
	int		temp_idx;

	temp_key = curr->key;
	curr->key = next->key;
	next->key = temp_key;
	temp_value = curr->value;
	curr->value = next->value;
	next->value = temp_value;
	temp_idx = curr->idx;
	curr->idx = next->idx;
	next->idx = temp_idx;
}

void	restore_envp_order(t_main_node *main)
{
	t_envp_node	*curr;
	int			i;
	int			j;

	i = 0;
	curr = main->ev_lst->next;
	while (curr)
	{
		curr = curr->next;
		i++;
	}
	while (i)
	{
		j = 0;
		curr = main->ev_lst->next;
		while (curr->next && j < i)
		{
			if (curr->idx > curr->next->idx)
				swap_envp_node(curr, curr->next);
			curr = curr->next;
			j++;
		}
		i--;
	}
}

void	envp_ascending_order(t_main_node *main)
{
	t_envp_node	*curr;
	int			i;
	int			j;

	i = 0;
	curr = main->ev_lst->next;
	while (curr)
	{
		curr->idx = i;
		curr = curr->next;
		i++;
	}
	while (i)
	{
		j = 0;
		curr = main->ev_lst->next;
		while (curr->next && j < i)
		{
			if (ft_strcmp(curr->key, curr->next->key) > 0)
				swap_envp_node(curr, curr->next);
			curr = curr->next;
			j++;
		}
		i--;
	}
}
