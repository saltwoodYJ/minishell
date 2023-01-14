/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   cd.c											   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/06 23:38:32 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/13 22:11:44 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

void	exec_chdir(t_main_node *main, char *path, char *old_path)
{
	char	*pwd;

	if (chdir(path) == 0)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			perror("minishell: pwd");
			main->status = 1;
			return ;
		}
		add_env(main, "PWD", pwd);
		add_env(main, "OLDPWD", old_path);
		free(pwd);
		free(old_path);
		main->status = 0;
	}
	else
	{
		perror_comment(main, "cd", path, 1);
		free(old_path);
	}
}

char	*get_cd_path(t_main_node *main, char *key)
{
	char	*path;

	path = get_env_path(main->ev_lst, key);
	if (!path)
	{
		error_msg(main, key, NOT_SET_ERROR, 1);
		return (0);
	}
	return (path);
}

void	ft_cd(t_main_node *main)
{
	char	*path;
	char	*old_path;
	char	**cmd;

	cmd = main->curr->cmd;
	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
		path = get_cd_path(main, "HOME");
	else if (!ft_strcmp(cmd[1], "-"))
	{
		path = get_cd_path(main, "OLDPWD");
		if (path)
			printf("%s\n", path);
	}
	else
		path = main->curr->cmd[1];
	if (!path)
		return ;
	old_path = getcwd(NULL, 0);
	exec_chdir(main, path, old_path);
}
