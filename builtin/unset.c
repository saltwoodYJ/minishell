/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:38:26 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/09 16:11:56 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env(t_main_node *main, char *key)
{
	t_envp_node	*pre;
	t_envp_node	*old;

	if (is_invalid_key(key, 1))
	{
		printf("minishell: unset: `%s': not a valid identifier\n", key);
		main->status = 1;
		return ;
	}
	pre = main->ev_lst->next;
	while (pre->next != NULL)
	{
		if (!ft_strcmp(pre->next->key, key))
		{
			old = pre->next;
			if (old == NULL)
				return ;
			pre->next = old->next;
			free(old->key);
			free(old->value);
			free(old);
			return ;
		}
		else
			pre = pre->next;
	}
}

void	ft_unset(t_main_node *main)
{
	char		**cmd;
	int			i;

	cmd = main->curr->cmd;
	i = 1;
	while (cmd[i])
	{
		remove_env(main, cmd[i]);
		i++;
	}
	main->status = 1;
}
