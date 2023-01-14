/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 04:06:32 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/14 22:20:57 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**make_envp_arr(t_envp_node *envp)
{
	char		**arr;
	int			len;
	int			i;
	t_envp_node	*curr;

	len = 0;
	curr = envp->next;
	while (curr != NULL)
	{
		len++;
		curr = curr->next;
	}
	arr = (char **)malloc(sizeof(char *) * (len + 1));
	curr = envp->next;
	i = 0;
	while (curr && i < len)
	{
		arr[i] = ft_strjoin3(curr->key, "=", curr->value);
		curr = curr->next;
		i++;
	}
	arr[i] = 0;
	return (arr);
}

char	**search_origin_path(t_envp_node *envp)
{
	int			i;
	t_envp_node	*curr;

	i = 0;
	curr = envp->next;
	while (curr != 0)
	{
		if (ft_strcmp(curr->key, "PATH") == 0)
			return (ft_split(curr->value, ':'));
		curr = curr->next;
	}
	return (NULL);
}

char	*get_path(char *first_cmd, char **splited_path)
{
	int		i;
	int		path_count;
	char	*temp_path;
	char	*cmd_path;

	path_count = ft_double_strlen(splited_path);
	temp_path = ft_strjoin("/", first_cmd);
	i = 0;
	while (i < path_count)
	{
		cmd_path = ft_strjoin(splited_path[i], temp_path);
		if (access(cmd_path, X_OK) == 0)
			break ;
		else
			free(cmd_path);
		i++;
	}
	free(temp_path);
	if (i == path_count)
		return (NULL);
	return (cmd_path);
}

char	*make_exec_path(t_main_node *main)
{
	char	**splited_path;
	char	*path;

	splited_path = search_origin_path(main->ev_lst);
	if (!splited_path)
		exit(error_msg(main, main->curr->cmd[0], FILE_ERROR, 127));
	path = get_path(main->curr->cmd[0], splited_path);
	ft_free_str(splited_path, 0);
	return (path);
}

void	exec_non_builtin(t_main_node *main)
{
	char	*path;
	char	**envp_arr;

	if (no_cmd(main, 1))
		exit(0);
	if (ft_search_char(main->curr->cmd[0], '/') != -1)
		path = main->curr->cmd[0];
	else
		path = make_exec_path(main);
	if (!path || !ft_strcmp(main->curr->cmd[0], "\0"))
		exit(error_msg(main, main->curr->cmd[0], CMD_ERROR, 127));
	if (access(path, F_OK) == 0 && access(path, X_OK) == -1)
		exit(error_msg(main, path, PERMISSION_ERROR, 126));
	envp_arr = make_envp_arr(main->ev_lst);
	if (!envp_arr)
		exit(1);
	execve(path, main->curr->cmd, envp_arr);
	exit(perror_comment(main, NULL, path, 127));
}
