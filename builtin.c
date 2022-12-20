#include "minishell.h"

void ft_echo(t_cmd *cmd)
{

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

void ft_export(t_cmd *cmd)
{
	t_node *curr;

	curr = cmd->head->next;
	

}

void ft_unset(t_cmd *cmd)
{

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