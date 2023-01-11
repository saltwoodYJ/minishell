/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:07:26 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/11 15:32:01 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_line(char *limiter, int infile)
{
	char	*line;
	char	*line_nl;

	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			return (0);
		}
		line_nl = ft_strjoin(line, "\n");
		write(infile, line_nl, ft_strlen(line_nl));
		free(line_nl);
		free(line);
	}
}

char	*make_heredoc(char *limiter, int idx)
{
	int		tmp_file;
	char	*tmp_name;
	char	*itoa_idx;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		printf("fork_error");
	itoa_idx = ft_itoa(idx);
	tmp_name = ft_strjoin3(".heredoc", itoa_idx, ".tmp");
	free(itoa_idx);
	tmp_file = open(tmp_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (pid == 0)
	{
		read_line(limiter, tmp_file);
		exit(0);
	}
	if (pid > 0)
	{
		wait(NULL);
		close(tmp_file);
	}
	return (tmp_name);
}

void	set_heredoc(t_main_node *main)
{
	t_infile_node	*curr;
	char			*file_name;
	int				i;

	if (!main->heredoc_node)
		return ;
	curr = main->heredoc_node->hnext;
	i = 0;
	while (curr)
	{
		file_name = make_heredoc(curr->limiter, i);
		curr->file = file_name;
		curr = curr->hnext;
		i++;
	}
}
