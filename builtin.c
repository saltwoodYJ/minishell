#include "minishell.h"

void ft_echo(t_main_node *main)
{
	char **cmd;

	cmd = main->node_head->cmd; /* echo의 다음 인자 */
	if (ft_strcmp(cmd[1], "-n") == 0)
	{
		if (!cmd[2])
			printf("\n");
		else
			printf("%s", cmd[2]);
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
		if (ft_strcmp(envp[i], var) == 0)
		{
			free(var);
			return (envp[i] + var_len);
		}
		i++;
	}
	return (NULL);
}

void ft_cd(t_main_node *main)
{
	char	*path;

	if (!main->node_head->cmd[1]) /* cd만 있을경우 */
	{
		path = get_env_path(main->ev, "HOME");
		if (chdir(path) != 0) //실패
			printf("chdir exec error\n");
		return ;
	}
	// if (ft_strcmp(curr->cmd[0], "$", 2) == 0)
	// {
	// 	curr = curr->next; 
	// 	path = get_env_path(data, temp->str);
	// 	if (!path)
	// 		path = get_env_path(data, "HOME");
	// }
	else
		path = main->node_head->cmd[1]; //현재 위치에서 구해야
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

char	**add_env(t_main_node *main, char *value)
{
	char **new_envp;
	int	len;
	int value_len;
	int	i;

	i = 0;
	len = ft_double_strlen(main->ev);
	new_envp = (char **)malloc(sizeof(char *) * (len + 2));
	value_len = ft_strlen(value);
	while (main->ev[i])
	{
		if (ft_strcmp(main->ev[i], value) == '=') /* 이미 있는 키값 */
		{
			main->ev[i] = value;
			i = 0;
			while (i < len + 2)
			{
				free(new_envp[i]);
				i++;
			}
			return (NULL);
		}
		new_envp[i] = main->ev[i];
		i++;
	}
	new_envp[i] = value;
	new_envp[++i] = 0;
	return (new_envp);
}

void show_export(t_main_node *main)
{
	int i;

	i = 0;
	while (main->ev[i])
	{
		printf("declare -x %s\n", main->ev[i]);
		i++;
	}
}

void ft_export(t_main_node *main)
{
	char 	*arg;
	char	**temp;
	int		i;

	i = 0;
	if (main->node_head->cmd[1] == NULL)
	{
		show_export(main_node);
		return ;
	}
	arg = main->node_head->cmd[1]; /*export 다음 인자*/
	temp = add_env(main_node, arg);
	if (!temp)
		return ;
	// ft_free(main->ev, 0);
	main->ev = temp;
	// ft_env(main_node);
}

void ft_unset(t_main_node *main)
{
	char *cmd;
	char **new_envp;
	int	len;
	int	i;
	int	j;

	i = 0;
	j = 0;
	cmd = main->node_head->cmd[1]; /*export 다음 인자*/
	len = ft_double_strlen(main->ev);
	new_envp = (char **)malloc(sizeof(char *) * (len));
	while (main->ev[i])
	{
		if (ft_strcmp(main->ev[j], cmd) != '=')
		{
			new_envp[i] = main->ev[j];
			i++;
		}
		j++;
	}
	new_envp[i] = 0;
	main->ev = new_envp;
	// ft_env(main_node);
}

void ft_env(t_main_node *main)
{
	int i;

	i = 0;
	while (main->ev[i])
	{
		printf("%s\n", main->ev[i]);
		i++;
	}
}

void ft_exit(t_main_node *main)
{
	t_cmd_node *curr;
	int		exit_code;
	
	curr = main->node_head;
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
	main->status = 0;
	exit(0);
}

void ft_exit_code(t_main_node *main)
{
	printf("%d: command not found\n", main->status); //status 저장
}

void exec_builtin(t_main_node *main)
{
    t_cmd_node *curr;

    curr = main->node_head;
    if (ft_strcmp(curr->cmd[0], "echo") == 0)
		ft_echo(main_node);
	else if (ft_strcmp(curr->cmd[0], "cd") == 0)
		ft_cd(main_node);
	else if (ft_strcmp(curr->cmd[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(curr->cmd[0], "export") == 0)
		ft_export(main_node);
	else if (ft_strcmp(curr->cmd[0], "unset") == 0)
		ft_unset(main_node);
	else if (ft_strcmp(curr->cmd[0], "env") == 0)
		ft_env(main_node);
	else if (ft_strcmp(curr->cmd[0], "exit") == 0)
		ft_exit(main_node);
	else if (ft_strcmp(curr->cmd[0], "$?") == 0)
		ft_exit_code(main_node);
}