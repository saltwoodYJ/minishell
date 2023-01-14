/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   parse_red.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yejinam <marvin@42.fr>					 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/07 01:34:38 by yejinam		   #+#	#+#			 */
/*   Updated: 2023/01/07 01:39:38 by yejinam		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../header/minishell.h"

void	set_red_lst(t_parsing_node *parse, t_cmd_node *node, t_main_node *main)
{
	t_parsing_node	*p_now;

	p_now = parse->next;
	while (p_now && p_now->type != PIPE)
	{
		if (p_now->type == RED_A || p_now->type == RED_H
			|| p_now->type == RED_I || p_now->type == RED_O)
		{
			if (p_now->next == NULL || p_now->next->type != WORD)
				return ;
			set_red(p_now, node, main);
		}
		p_now = p_now->next;
	}
}

void	set_red(t_parsing_node *parsing, t_cmd_node *node, t_main_node *main)
{
	t_type	type;
	void	*red_node;

	type = parsing->type;
	if (type == RED_I || type == RED_H)
	{
		red_node = (t_infile_node *)ft_malloc(sizeof(t_infile_node));
		set_infile_node(parsing, (t_infile_node *)red_node);
		if (type == RED_H)
			append_heredoc_node(main->heredoc_node, red_node);
		append_infile_node(node->infile_node, red_node);
		return ;
	}
	red_node = (t_outfile_node *)ft_malloc(sizeof(t_outfile_node));
	set_outfile_node(parsing, red_node);
	append_outfile_node(node->outfile_node, red_node);
}

void	set_infile_node(t_parsing_node *parsing, void *node)
{
	t_infile_node	*file_node;

	file_node = (t_infile_node *)node;
	if (parsing->type == RED_H)
	{
		file_node->is_heredoc = 1;
		file_node->limiter = parsing->next->str;
	}
	else
	{	
		file_node->file = parsing->next->str;
		file_node->is_heredoc = 0;
	}
	file_node->hnext = NULL;
	file_node->next = NULL;
}

void	set_outfile_node(t_parsing_node *parsing, void *node)
{
	t_outfile_node	*file_node;

	file_node = (t_outfile_node *)node;
	file_node->type = 0;
	if (parsing->type == RED_A)
		file_node->type = 1;
	file_node->file = parsing->next->str;
	file_node->next = NULL;
}

void	*ft_malloc(int size)
{
	void	*red_node;

	red_node = ft_calloc(1, size);
	if (!red_node)
		exit (1);
	return (red_node);
}
