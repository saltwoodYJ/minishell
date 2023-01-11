/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:38:21 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/11 16:44:23 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envp_node	*make_envp_node(char *key, char *value)
{
	t_envp_node	*new;

	new = (t_envp_node *)malloc(sizeof(t_envp_node));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	add_env(t_main_node *main, char *key, char *value)
{
	t_envp_node	*curr;
	t_envp_node	*pre;

	curr = main->ev_lst->next;
	pre = main->ev_lst;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			if (!value)
				return ;
			curr->value = value;
			return ;
		}
		curr = curr->next;
		pre = pre->next;
	}
	pre->next = make_envp_node(key, value);
}

void	show_export(t_main_node *main)
{
	t_envp_node	*curr;

	curr = main->ev_lst->next;
	while (curr != NULL)
	{
		if (!curr->value)
			printf("declare -x %s\n", curr->key);
		else
			printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
		curr = curr->next;
	}
	main->status = 0;
}

void	ft_export(t_main_node *main)
{
	char		**cmd;
	char		**args;
	int			i;

	cmd = main->curr->cmd;
	i = 1;
	if (!cmd[i])
		show_export(main);
	while (cmd[i])
	{	
		if (search_equal(cmd[i]) == 0 || is_invalid_key(cmd[i], 0)) 
			error_msg(main, cmd[i], EXPORT_KEY_ERROR, 1);
		else if (search_equal(cmd[i]) == -1)
		{
			add_env(main, cmd[i], NULL);
			main->status = 0;
		}	
		else
		{
			args = make_key_value(cmd[i]);
			add_env(main, args[0], args[1]);
			main->status = 0;
		}
		i++;
	}
}
