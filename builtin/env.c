/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:38:28 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/07 02:54:55 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_main_node *main)
{
	t_envp_node	*curr;

	curr = main->ev_lst->next;
	while (curr != NULL && curr->value != NULL)
	{
		printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	main->status = 0;
}