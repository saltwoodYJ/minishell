/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 14:46:03 by yejinam           #+#    #+#             */
/*   Updated: 2023/01/07 05:56:45 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_main(t_main_node *main, char *line)
{
	make_token(line, main);
	main->status = 0;
	main->stdin_fd = dup(0);
	main->stdout_fd = dup(1);
	main->curr = main->node_head->next;
	set_heredoc(main);
}

void	restore_std(t_main_node *main)
{
	dup2(main->stdin_fd, 0);
	dup2(main->stdout_fd, 1);
}

int	main(int ac, char **av, char **ev)
{
	char		*line;
	t_main_node	*main;
	t_envp_node	*ev_lst;

	if (ac < 0 || av[0] == NULL)
		return (0);
	ev_lst = parse_envp(ev);
	main = malloc(sizeof(t_main_node));
	while (1)
	{
		line = readline("minishell$ ");
		if (line)
		{
			make_main(main, line);
			main->ev_lst = ev_lst;
			add_history(line);
			// run_command(main);
			restore_std(main);
			free_main(main);
			free(main);
			main = NULL;
			free(line);
			system("leaks minishell");
		}
	}
	return (main->status);
}
