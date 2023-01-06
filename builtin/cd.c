/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:38:32 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/07 05:00:37 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_chdir(t_main_node *main, char *path, char *old_path)
{
	char	*pwd;

	if (chdir(path) == 0)
	{
		pwd = getcwd(NULL, 0);
		add_env(main, "PWD", pwd);
		add_env(main, "OLDPWD", old_path);
		main->status = 0;
		free(pwd);
	}
	else
	{
		perror_comment("cd", main->curr->cmd[1]);
		main->status = 1;
	}
}

void	ft_cd(t_main_node *main)
{
	char	*path;
	char	*old_path;
	char	**cmd;

	cmd = main->curr->cmd;
	old_path = getcwd(NULL, 0);
	if (!cmd[1] || !ft_strcmp(cmd[1], "~")) /* cd만 있을경우 */
	{
		path = get_env_path(main->ev_lst, "HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set");
			main->status = 1;
			free(old_path);
			return ;
		}
	}
	else if (!ft_strcmp(cmd[1], "-"))
	{
		path = get_env_path(main->ev_lst, "OLDPWD"); //OLDPWD가 없으면?
		printf("%s\n", path);
	}
	else
		path = main->curr->cmd[1]; //현재 위치에서 구해야
	exec_chdir(main, path, old_path);
}
