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

void	*ft_free(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

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
    		ft_free(node->cmd[i]);
			i++;
		}
		ft_free(node->cmd);
	}
    // ft_input_clear(&(node->infile_node));
    // ft_output_clear(&(node->outfile_node));
	ft_free(node);
}

void	free_double_char(char **cmd)
{
	int		i;
	char	*str;

	i = 0;
	str = cmd[i];
	while (cmd[i])
	{
		ft_free(cmd[i]);
		i++;
	}
	ft_free(cmd);
}

void	outfile_node_clear(t_outfile_node	*nodes)
{
	t_outfile_node	*tmp;

	if (nodes == 0)
		return ;
	while (nodes)
	{
		tmp = nodes;
		nodes = nodes->next;
		ft_free(tmp->file);
		ft_free(tmp);
	}
}

void	infile_node_clear(t_infile_node	*nodes)
{
	t_infile_node	*tmp;

	if (nodes == 0)
		return ;
	while (nodes)
	{
		tmp = nodes;
		nodes = nodes->next;
		ft_free(tmp->file);
		ft_free(tmp->limiter);
		ft_free(tmp);
	}
}

void	cmd_node_clear(t_cmd_node	*nodes)
{
	t_cmd_node	*tmp;

	if (nodes == 0)
		return ;
	while (nodes)
	{
		tmp = nodes;
		nodes = nodes->next;
		infile_node_clear(tmp->infile_node);
		outfile_node_clear(tmp->outfile_node);
		free_double_char(tmp->cmd);
		ft_free(tmp);
	}
}

void	free_main(t_main_node *main)
{
	(void)main;
//    cmd_node_clear(main->node_head);
	//free(main->node_head);
	ft_input_clear(&(main->heredoc_node));
	//free(main->heredoc_node);
}
