/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   free_main.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/07 05:18:52 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/11 13:47:47 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

void	outfile_node_clear(t_outfile_node	*nodes)
{
	t_outfile_node	*tmp;

	if (nodes == 0)
		return ;
	while (nodes)
	{
		tmp = nodes;
		nodes = nodes->next;
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
		if (tmp->is_heredoc)
			ft_free(tmp->file);
		ft_free(tmp);
	}
}

void	cmd_node_clear(t_cmd_node *nodes)
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
		ft_free(tmp->cmd);
		ft_free(tmp);
	}
}

void	parse_node_clear(t_parsing_node	*nodes)
{
	t_parsing_node	*tmp;

	if (nodes == 0)
		return ;
	while (nodes)
	{
		tmp = nodes;
		nodes = nodes->next;
		ft_free(tmp->str);
		ft_free(tmp);
	}
}

void	free_main(t_main_node *main)
{
	cmd_node_clear(main->node_head);
	parse_node_clear(main->parse);
	ft_free(main->heredoc_node);
	ft_free(main);
}
