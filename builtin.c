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
	char	*old_path;
	char	*pwd;
	char	*old_pwd;
	
	old_path = path = getcwd(NULL, 0);
	if (!main->node_head->cmd[1]) /* cd만 있을경우 */
	{
		path = get_env_path(main->ev, "HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set");
			main->status = 1;
			free(old_path);
			return ;
		}
	}
	else
		path = main->node_head->cmd[1]; //현재 위치에서 구해야
	if (chdir(path) == 0) //성공
	{
		pwd = ft_strjoin("PWD=", path, 0, 0);
		add_env(main, pwd);
		old_pwd = ft_strjoin("OLDPWD=", old_path, 0, 0);
		add_env(main, old_pwd);
		free(pwd);
		free(old_pwd);
		main->status = 0;
	}
	else
	{
		printf("minishell: cd: %s : No such file or directory\n", path);
		main->status = 1;
	}
	free(path);
	free(old_path);
}

void ft_pwd()
{
	char	*path;
	
	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		printf("Error\n");
		return ;
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
		show_export(main);
		return ;
	}
	arg = main->node_head->cmd[1]; /*export 다음 인자*/
	temp = add_env(main, arg);
	if (!temp)
		return ;
	// ft_free(main->ev, 0);
	main->ev = temp;
	// ft_env(main);
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
	// ft_env(main);
}

void ft_env(t_main_node *main)
{
	int i;

	i = 0;
	while (main->ev[i])
	{
		if (ft_strrchr(main->ev[i], '=') != 0)
			printf("%s\n", main->ev[i]);
		i++;
	}
}

void ft_exit(t_main_node *main)
{
	t_cmd_node *curr;
	int		exit_code;
	int		is_char;
	
	curr = main->node_head;
	/*종료 코드 저장!! */
	if (curr->cmd[1])  //인자가 두개일 때 
	{
		exit_code = ft_exit_atoi(curr->cmd[1], &is_char);
		if (is_char == 1)
		{
			printf("exit\nminishell: exit: %s: numeric argument required", curr->cmd[1]);
			main->status = 255;
			exit(255);
		}
		else if (curr->cmd[2])
		{
			printf("exit\nminishell: exit: too many arguments");
			main->status = 1;
		}
		else if (exit_code > 255)
		{
			printf("exit: %s: undefined exit code\n", curr->cmd[1]);
			//exit code는 어떻게..?
			exit(0);
		}
		else
		{
			main->status = exit_code;
			exit(exit_code);
		}
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
		ft_echo(main);
	else if (ft_strcmp(curr->cmd[0], "cd") == 0)
		ft_cd(main);
	else if (ft_strcmp(curr->cmd[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(curr->cmd[0], "export") == 0)
		ft_export(main);
	else if (ft_strcmp(curr->cmd[0], "unset") == 0)
		ft_unset(main);
	else if (ft_strcmp(curr->cmd[0], "env") == 0)
		ft_env(main);
	else if (ft_strcmp(curr->cmd[0], "exit") == 0)
		ft_exit(main);
}