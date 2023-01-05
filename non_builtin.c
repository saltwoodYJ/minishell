
#include "minishell.h"

char	**search_origin_path(t_envp_node *envp)
{
	int		i;
	t_envp_node *curr;

	i = 0;
	curr = envp->head->next;
	while (curr != 0)
	{
		if (ft_strcmp(curr->key, "PATH") == 0)
			return (ft_split(curr->value, ':'));
		curr = curr->next;
	}
	return (0);
}

char	*get_path(t_envp_node *envp, char *first_cmd)
{
	int		i;
	int		path_count;
	char	*temp_path;
	char	**splited_path;
	char	*cmd_path;

	if (access(first_cmd, X_OK) == 0)
		return (first_cmd);
	splited_path = search_origin_path(envp);
	path_count = ft_double_strlen(splited_path);
	temp_path = ft_strjoin("/", first_cmd);
	i = 0;
	while (i < path_count)
	{
		cmd_path = ft_strjoin(splited_path[i], temp_path);
		if (access(cmd_path, X_OK) == 0)
			break ;
		else
			ft_free(0, &cmd_path);
		i++;
	}
	// ft_free(splited_path, 0);
	// ft_free(0, &temp_path);
	return (cmd_path);
}

char	**make_envp_arr(t_envp_node *envp)
{
	char		**arr;
	char		*temp;
	int			len;
	int			i;
	t_envp_node *curr;

	len = 0;
	curr = envp->head->next;
	while (curr != NULL)
	{
		len++;
		curr = curr->next;
	}
	arr = (char **)malloc(sizeof(char *) * (len + 1));
	curr = envp->head->next;
	i = 0;
	while (i < len)
	{
		arr[i] = ft_strjoin3(curr->key, "=", curr->value);
		free(temp);
		i++;
	}
	arr[i] = 0;
	return (arr);
}

void exec_non_builtin(t_main_node *main)
{
	char 	**cmd_args;
	char	*path;
	char	**envp_arr;

	/* cmd 합치기 */
	// cmd_args = search_cmd(cmd);
	// if (!cmd)
	// 	return ; //오류 처리
	cmd_args = main->curr->cmd;
	path = get_path(main->envp, cmd_args[0]);
	if (!path)
	{
		printf("minishell: command not found: %s\n", cmd_args[0]);
		main->status = 127;
		ft_free(cmd_args, 0); /* 문제!! */
		exit(127);
	}
	envp_arr = make_envp_arr(main->envp);
	execve(path, cmd_args, envp_arr);
	perror("minishell :");
	if (errno == ENOEXEC) //errno 처리
        exit(126);
    else if(errno == ENOENT) //errno 처리
        exit(127);
    else
        exit(EXIT_FAILURE);
}
