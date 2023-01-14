/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   minishell.c								:+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: hyeokim2 <hyeokim2@student.42.fr>		+#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2022/12/15 14:46:03 by yejinam		 #+#	#+#		   */
/*   Updated: 2023/01/14 19:36:14 by hyeokim2		###   ########.fr	  */
/*															 */
/* ************************************************************************** */

#include "minishell.h"

void	make_main(t_main_node **main, char *line, int status, \
t_envp_node *ev_lst)
{
	(*main) = ft_malloc(sizeof(t_main_node));
	if (check_quote_err(line))
	{
		(*main)->status = 258;
		(*main)->cmd_num = -1;
		return ;
	}
	(*main)->heredoc_node = ft_malloc(sizeof(t_infile_node));
	(*main)->node_head = ft_malloc(sizeof(t_cmd_node));
	(*main)->ev_lst = ev_lst;
	(*main)->status = status;
	make_token(line, (*main));
	(*main)->stdin_fd = dup(0);
	(*main)->stdout_fd = dup(1);
	(*main)->curr = (*main)->node_head->next;
	set_heredoc(*main);
}

void	restore_std(t_main_node *main)
{
	dup2(main->stdin_fd, 0);
	dup2(main->stdout_fd, 1);
}

extern char	**environ;

int	main(void)
{
	char			*line;
	t_main_node		*main;
	t_envp_node		*ev_lst;
	int				status;

	ev_lst = parse_envp(environ);
	status = 0;
	while (1)
	{
		rm_ctrl();
		set_signal(2,2);
		line = readline("minishell$ ");
		if (!line)
			exit(0);
		if (line[0])
		{
			make_main(&main, line, status, ev_lst);
			add_history(line);
			if (main->cmd_num != -1)
			{
				run_command(main);
				restore_std(main);
				clear_heredoc(main);
			}
			status = main->status;
			free_main(main);
			system("leaks minishell");
		}
		ft_free(line);
	}
	return (status);
}
