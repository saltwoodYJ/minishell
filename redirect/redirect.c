/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:07:20 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/11 13:47:02 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_heredoc(t_main_node *main)
{
	t_infile_node	*curr;

	curr = main->curr->infile_node;
	while (curr)
	{
		if (curr->is_heredoc == 1)
			unlink(curr->file);
		curr = curr->next;
	}
}

int	input_redirect(t_main_node *main)
{
	t_infile_node	*curr;
	int				input_fd;

	curr = main->curr->infile_node;
	while (curr)
	{
		if (curr->file)
		{
			input_fd = open(curr->file, O_RDONLY);
			if (input_fd >= 0)
			{
				dup2(input_fd, 0);
				close(input_fd);
			}
			else
				return (error_msg(main, curr->file, FILE_ERROR, 1));
		}
		curr = curr->next;
	}
	return (0);
}

int	output_redirect(t_main_node *main)
{
	t_outfile_node	*curr;
	int				output_fd;

	curr = main->curr->outfile_node;
	while (curr)
	{
		if (curr->file)
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
			else
				return (error_msg(main, curr->file, FILE_ERROR, 1));
		}
		curr = curr->next;
	}
	return (0);
}
