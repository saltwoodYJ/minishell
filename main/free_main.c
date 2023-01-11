/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 05:18:52 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/11 13:47:47 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_one_node(t_cmd_node *node)
{
    int i;

    i = 0;
	if (node == 0)
		return ;
	if (node->cmd)
	{
    	while (node->cmd[i])
		{
    		free(node->cmd[i]);
			i++;
		}
		free(node->cmd);
	}
    // ft_input_clear(&(node->infile_node));
    // ft_output_clear(&(node->outfile_node));
	free(node);
}

void	ft_node_clear(t_cmd_node **nodes)
{
	t_cmd_node	*curr;

	if (nodes == 0)
		return ;
	while (*nodes)
	{
		curr = (*nodes)->next;
		free_one_node(*nodes);
		*nodes = curr;
	}
	*nodes = 0;
}

void	free_main(t_main_node *main)
{
    ft_node_clear(&(main->node_head));
	free(main->node_head);
	ft_input_clear(&(main->heredoc_node));
	free(main->heredoc_node);
}
