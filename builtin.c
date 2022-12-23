#include "minishell.h"

void ft_echo(t_cmd *cmd)
{
	t_node *curr;

	curr = cmd->head->next->next; /* echo의 다음 인자 */
	if (ft_strncmp(curr->str, "-n", 3) == 0)
	{
		if (!curr->next)
			printf("\n");
		else
			printf("%s", curr->next->str);
	}
	else
		printf("%s\n", curr->str);
}

char *get_env_path(char **envp, char *str)
{
	char *var;
	int var_len;
	int i;

	i = 0;
	var = ft_strjoin(str, "=", 0, 0);
	var_len = ft_strlen(var);
	while (envp[i] != 0)
	{
		if (ft_strncmp(envp[i], var, var_len) == 0)
		{
			free(var);
			return (envp[i] + var_len);
		}
		i++;
	}
	return (NULL);
}

void ft_cd(t_cmd *cmd)
{
	t_node	*curr;
	char	*path;

	curr = cmd->head->next->next;
	if (!curr) /* cd만 있을경우 */
	{
		path = get_env_path(cmd->info->envp, "HOME");
		if (chdir(path) != 0) //실패
			printf("ch error\n");
		return ;
	}
	// if (ft_strncmp(curr->str, "$", 2) == 0)
	// {
	// 	curr = curr->next; 
	// 	path = get_env_path(data, temp->str);
	// 	if (!path)
	// 		path = get_env_path(data, "HOME");
	// }
	else
		path = curr->str; //현재 위치에서 구해야
	if (chdir(path) != 0) //실패
		printf("cd: %s : No such file or directory\n", path);
}

void ft_pwd()
{
	char	*path;
	
	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		printf("Error\n");
		exit(1);
	}
	printf("%s\n", path);
	free(path);
}

char	**add_env(t_cmd *cmd, char *value)
{
	char **new_envp;
	int	len;
	int value_len;
	int	i;

	i = 0;
	len = ft_double_strlen(cmd->info->envp);
	new_envp = (char **)malloc(sizeof(char *) * (len + 2));
	value_len = ft_strlen(value);
	while (cmd->info->envp[i])
	{
		if (ft_strncmp(cmd->info->envp[i], value, value_len+1) == '=') /* 이미 있는 키값 */
		{
			cmd->info->envp[i] = value;
			i = 0;
			while (i < len + 2)
			{
				free(new_envp[i]);
				i++;
			}
			return (NULL);
		}
		new_envp[i] = cmd->info->envp[i];
		i++;
	}
	new_envp[i] = value;
	new_envp[++i] = 0;
	return (new_envp);
}

void ft_export(t_cmd *cmd)
{
	t_node *curr;
	char	**temp;
	int		i;

	i = 0;
	curr = cmd->head->next->next; /*export 다음 인자*/
	temp = add_env(cmd, curr->str);
	if (!temp)
		return ;
	// ft_free(cmd->info->envp, 0);
	cmd->info->envp = temp;
	ft_env(cmd);
}

void ft_unset(t_cmd *cmd)
{
	t_node *curr;
	char **new_envp;
	int	len;
	int	i;
	int	j;

	i = 0;
	j = 0;
	curr = cmd->head->next->next; /*export 다음 인자*/
	len = ft_double_strlen(cmd->info->envp);
	new_envp = (char **)malloc(sizeof(char *) * (len));
	len = ft_strlen(curr->str);
	while (cmd->info->envp[j])
	{
		if (ft_strncmp(cmd->info->envp[j], curr->str, len + 1) != '=')
		{
			new_envp[i] = cmd->info->envp[j];
			i++;
		}
		j++;
	}
	new_envp[i] = 0;
	cmd->info->envp = new_envp;
	ft_env(cmd);
}

void ft_env(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->info->envp[i])
	{
		printf("%s\n", cmd->info->envp[i]);
		i++;
	}
}

void ft_exit2(t_cmd *cmd)
{
	t_node *curr;
	int		exit_code;
	
	curr = cmd->head->next->next;
	/*종료 코드 저장!! */
	if (curr && curr->str)
	{
		exit_code = ft_atoi(curr->str);
		if (exit_code == -1)
			printf("exit: %s: numeric argument required\n", curr->str);
		// if (exit_code > 255)
	}
		exit(curr->str);
	exit(0);
}

void exec_builtin(t_cmd *cmd)
{
    t_node *curr;

    curr = cmd->head->next;
    if (ft_strncmp(curr->str, "echo", 5) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(curr->str, "cd", 3) == 0)
		ft_cd(cmd);
	else if (ft_strncmp(curr->str, "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(curr->str, "export", 7) == 0)
		ft_export(cmd);
	else if (ft_strncmp(curr->str, "unset", 6) == 0)
		ft_unset(cmd);
	else if (ft_strncmp(curr->str, "env", 4) == 0)
		ft_env(cmd);
	else if (ft_strncmp(curr->str, "exit", 5) == 0)
		ft_exit2(cmd);
}