
#include "minishell.h"

char	**search_cmd(t_data* data)
{
	char	**cmd;
	t_node	*temp;
	int		idx;

	temp = data->curr;
	idx = 0;
	while (temp != NULL && temp->type == WORD)
	{
		idx++;
		temp = temp->next;
	}
	cmd = (char **)malloc(sizeof(char *) * idx + 1);
	idx = 0;
	while (data->curr != NULL &&  data->curr->type == WORD)
	{
		cmd[idx] =  data->curr->str;
		idx++;
		data->curr = data->curr->next;
	}
	cmd[idx] = 0;
	return (cmd);
}

char	**search_origin_path(char **envp)
{
	int		i;
	char	*origin_path;

	i = 0;
	while (envp[i] != 0)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			origin_path = envp[i] + 5;
			return (ft_split(origin_path, ':'));
		}
		i++;
	}
	return (0);
}

char	*get_path(char **envp, char *first_cmd)
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
	temp_path = ft_strjoin("/", first_cmd, 0, 0);
	i = 0;
	while (i < path_count)
	{
		cmd_path = ft_strjoin(splited_path[i], temp_path, 0, 0);
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


void exec_nonbuiltin(t_data *data)
{
	char 	**cmd;
	char	*path;

	/* cmd 합치기 */
	cmd = search_cmd(data);
	if (!cmd)
		return ; //오류 처리

	path = get_path(data->envp, cmd[0]);
	if (!path)
	{
		ft_putstr_err(cmd[0], ": command not found");
		// ft_free(cmd, 0); /* 문제!! */
		exit(127);
	}
	execve(path, cmd, data->envp);
	write(2, "execve error\n", 13);
	exit(0);
}
