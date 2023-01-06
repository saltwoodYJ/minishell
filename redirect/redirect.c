/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:07:20 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/07 02:32:44 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	input_redirect(t_main_node *main)
{
	t_infile_node	*curr;
	int				input_fd;

	curr = main->curr->infile_node;
	while (curr != NULL)
	{
		input_fd = open(curr->file, O_RDONLY);
		if (input_fd >= 0)
		{
			dup2(input_fd, 0);
			close(input_fd);
			if (curr->is_heardoc == 1)
				unlink(curr->file);
		}
		curr = curr->next;
	}
}

void	output_redirect(t_main_node *main)
{
	t_outfile_node	*curr;
	int				output_fd;

	curr = main->curr->outfile_node;
	while (curr != NULL)
	{
		if (curr->type == 0)
			output_fd = open(curr->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		else
			output_fd = open(curr->file, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (output_fd >= 0)
		{
			dup2(output_fd, 1);
			close(output_fd);
		}
		curr = curr->next;
	}
}
