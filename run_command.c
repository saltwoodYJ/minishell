
#include "minishell.h"

char	**search_cmd(t_node *curr)
{
	char	**cmd;
	t_node	*temp;
	int		idx;

	temp = curr;
	idx = 0;
	while (temp != NULL && temp->type == WORD)
	{
		idx++;
		temp = temp->next;
	}
	cmd = (char **)malloc(sizeof(char *) * idx + 1);
	idx = 0;
	while (curr != NULL && curr->type == WORD)
	{
		cmd[idx] = curr->str;
		idx++;
		curr = curr -> next;
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

char	*get_path(char **envp, char *first_cmd, char *path)
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
	ft_free(splited_path, 0);
	ft_free(0, &temp_path);
	return (cmd_path);
}

void	ft_putstr_err(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return ;
	i = 0;
	while (s1[i] != 0)
	{
		write(2, &s1[i], 1);
		i++;
	}
	i = 0;
	while (s2[i] != 0)
	{
		write(2, &s2[i], 1);
		i++;
	}
	write(2, "\n", 1);
}

void make_exec(t_node *curr, char **envp)
{
	char 	**cmd;
	char	*path;

	/* cmd 합치기 */
	cmd = search_cmd(curr);
	if (!cmd)
	{
		return ; //오류 처리
	}
	path = get_path(envp, cmd[0], path);
	printf("path: %s", path);
	if (!path)
	{
		ft_putstr_err(cmd[0], ": command not found");
		ft_free(cmd, 0);
		exit(127);
	}
	execve(path, cmd, envp);
	write(2, "execve error\n", 13);
	exit(0);
}

int run_command(t_node *head, char **envp)
{
	t_node *curr;
	
	while (curr != NULL)
	{
		/* 살행 가능한 빌트인 함수인지 확인 */
		make_exec(curr, envp);
	}
	return (0);
}
