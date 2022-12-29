#include "minishell.h"

void ft_echo(t_main_node *main_node)
{
	char **cmd;

	cmd = main_node->node_head->cmd; /* echo의 다음 인자 */
	if (ft_strncmp(cmd[0], "-n", 3) == 0)
	{
		if (!cmd[1])
			printf("\n");
		else
			printf("%s", cmd[1]);
	}
	else
		printf("%s\n", cmd[1]);
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

void ft_cd(t_main_node *main_node)
{
	char	*arg;
	char	*path;

	arg = main_node->node_head->cmd[1];
	if (!arg) /* cd만 있을경우 */
	{
		path = get_env_path(main_node->ev, "HOME");
		if (chdir(path) != 0) //실패
			printf("ch error\n");
		return ;
	}
	// if (ft_strncmp(curr->cmd[0], "$", 2) == 0)
	// {
	// 	curr = curr->next; 
	// 	path = get_env_path(data, temp->str);
	// 	if (!path)
	// 		path = get_env_path(data, "HOME");
	// }
	else
		path = arg; //현재 위치에서 구해야
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
		return ; //안돼
	}
	printf("%s\n", path);
	free(path);
}

char	**add_env(t_main_node *main_node, char *value)
{
	char **new_envp;
	int	len;
	int value_len;
	int	i;

	i = 0;
	len = ft_double_strlen(main_node->ev);
	new_envp = (char **)malloc(sizeof(char *) * (len + 2));
	value_len = ft_strlen(value);
	while (main_node->ev[i])
	{
		if (ft_strncmp(main_node->ev[i], value, value_len+1) == '=') /* 이미 있는 키값 */
		{
			main_node->ev[i] = value;
			i = 0;
			while (i < len + 2)
			{
				free(new_envp[i]);
				i++;
			}
			return (NULL);
		}
		new_envp[i] = main_node->ev[i];
		i++;
	}
	new_envp[i] = value;
	new_envp[++i] = 0;
	return (new_envp);
}

void ft_export(t_main_node *main_node)
{
	char 	*arg;
	char	**temp;
	int		i;

	i = 0;
	arg = main_node->node_head->cmd[1]; /*export 다음 인자*/
	temp = add_env(main_node, arg);
	if (!temp)
		return ;
	// ft_free(main_node->ev, 0);
	main_node->ev = temp;
	// ft_env(main_node);
}

void ft_unset(t_main_node *main_node)
{
	char *cmd;
	char **new_envp;
	int	len;
	int	i;
	int	j;

	i = 0;
	j = 0;
	cmd = main_node->node_head->cmd[1]; /*export 다음 인자*/
	len = ft_double_strlen(main_node->ev);
	new_envp = (char **)malloc(sizeof(char *) * (len));
	len = ft_strlen(cmd);
	while (main_node->ev[i])
	{
		if (ft_strncmp(main_node->ev[j], cmd, len + 1) != '=')
		{
			new_envp[i] = main_node->ev[j];
			i++;
		}
		j++;
	}
	new_envp[i] = 0;
	main_node->ev = new_envp;
	// ft_env(main_node);
}

void ft_env(t_main_node *main_node)
{
	int i;

	i = 0;
	while (main_node->ev[i])
	{
		printf("%s\n", main_node->ev[i]);
		i++;
	}
}

void ft_exit(t_main_node *main_node)
{
	t_node *curr;
	int		exit_code;
	
	curr = main_node->node_head;
	/*종료 코드 저장!! */
	if (curr->cmd[1])
	{
		exit_code = ft_atoi(curr->cmd[1]);
		if (exit_code == -1)
			printf("exit: %d: numeric argument required\n", 255);
		if (exit_code > 255)
			printf("exit: %s: undefined exit code\n", curr->cmd[1]);
		exit(exit_code);
	}
	main_node->status = 0;
	exit(0);
}

void ft_exit_code(t_main_node *main_node)
{
	printf("%d: command not found\n", main_node->status); //status 저장
}

void exec_builtin(t_main_node *main_node)
{
    t_node *curr;

    curr = main_node->node_head;
    if (ft_strncmp(curr->cmd[0], "echo", 5) == 0)
		ft_echo(main_node);
	else if (ft_strncmp(curr->cmd[0], "cd", 3) == 0)
		ft_cd(main_node);
	else if (ft_strncmp(curr->cmd[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(curr->cmd[0], "export", 7) == 0)
		ft_export(main_node);
	else if (ft_strncmp(curr->cmd[0], "unset", 6) == 0)
		ft_unset(main_node);
	else if (ft_strncmp(curr->cmd[0], "env", 4) == 0)
		ft_env(main_node);
	else if (ft_strncmp(curr->cmd[0], "exit", 5) == 0)
		ft_exit(main_node);
	else if (ft_strncmp(curr->cmd[0], "$?", 3) == 0)
		ft_exit_code(main_node);
}