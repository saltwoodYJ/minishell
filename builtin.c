#include "minishell.h"

void ft_echo(t_main_node *main)
{
	char **cmd;
	int		i;
	int		nl;

	cmd = main->curr->cmd; /* echo의 다음 인자 */
	i = 2;
	nl = 0;
	if (ft_strcmp(cmd[1], "-n") == 0)
		nl = 1;
	if (nl == 1 && !cmd[2])
	{
		printf("\n");
		return ;
	}
	while (cmd[i])
	{
		printf(" %s", cmd[i]);
		i++;
	}
	if (nl != 1)
		printf("\n");
}

char *get_env_path(t_envp_node *envp, char *key)
{
	t_envp_node *curr;

	curr = envp->head->next;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->key, key) == 0)
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}

void ft_cd(t_main_node *main)
{
	char	*path;
	char	*old_path;
	
	old_path = getcwd(NULL, 0);
	if (!main->curr->cmd[1]) /* cd만 있을경우 */
	{
		path = get_env_path(main->envp, "HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set");
			main->status = 1;
			free(old_path);
			return ;
		}
	}
	else
		path = main->curr->cmd[1]; //현재 위치에서 구해야
	if (chdir(path) == 0) //성공
	{
		add_env(main, "PWD", path);
		add_env(main, "OLDPWD", old_path);
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

t_envp_node *make_envp_node(char *key, char *value)
{
	t_envp_node *new;

	new = (t_envp_node *)malloc(sizeof(t_envp_node));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	add_env(t_main_node *main, char *key, char *value)
{
	t_envp_node *curr;

	//value가 없으면 NULL로 들어감
	//value 있어야하나 값이 없으면 '\0'로]
	curr = main->envp->head->next;
	while (curr != NULL)
	{
		//변수에 숫자없는지 확인
		if (ft_strcmp(curr->key, key)) /* 이미 있는 키값 */
		{
			curr->value = value;
			return ;
		}
		curr = curr->next;
	}
	curr->next = make_envp_node(key, value);
}

void show_export(t_main_node *main)
{
	t_envp_node *curr;

	curr = main->envp->head->next;
	while (curr != NULL)
	{
		if (!curr->value)
			printf("declare -x %s\n", curr->key);
		else
			printf("declare -x %s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
}

int	search_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (i);
	}
	return (-1);
}

void	ft_export(t_main_node *main)
{
	char		**cmd;
	char	 	**args;
	int			i;

	cmd = main->curr->cmd;
	i = 1;
	if (!cmd[i]) //export 다음 인자
		show_export(main);
	while (cmd[i])
	{	
		if (search_equal(cmd[i]) == -1)
			add_env(main, cmd[i], NULL);
		else if (search_equal(cmd[i]) == 0)
			printf("minishell: export: `%s': not a valid identifier", cmd[i]);
		else
		{			
			args = ft_split(cmd[i], "="); /*export =는 어떻게 구분하지?*/
			if (is_number(args[0]))
				printf("minishell: export: `%s': not a valid identifier", cmd[i]);
			else
			{
				// if (args[1] == NULL)
				// 	args[1] = "\0";
				add_env(main, args[0], args[1]);
			}
		}
		i++;
	}

	//만약 value가 여러개 오면 뒤에껀 무시
	//만약 =은 있고 value가 없다면 없는 채로 들어감
	//=도 없고 value도 없다면 그냥 export에만 들어감
	// 1. =은 있지만 value가 없는 경우 -> value와 key 모두 저장 key는 "\0"이 들어감
	// 2-1. =도 없고 (이건 맨처음 검사) value도 없는 경우 -> 그냥 value없이 key만 저장
	// 2-2. export 뒤에 = 없이 여러개 들어오면 여러개가 그냥 선언됨.
	
	// ft_free(main->ev, 0);
	// main->ev = temp;
	// ft_env(main);
}

void delete_envp_node(t_envp_node *pre)
{
	t_envp_node *old;

	old = pre->next;
	if (old == NULL)
		return ;
	pre->next = old->next;
	free(old->key);
	free(old->value);
	free(old);
}

void ft_unset(t_main_node *main)
{
	t_envp_node *pre;
	char		**cmd;
	int			i;

	pre = main->envp->head;
	cmd = main->curr->cmd;
	i = 1;
	while (cmd[i])
	{
		while (pre->next != NULL)
		{
			if (!ft_strcmp(pre->next->key, cmd[i]))
				delete_envp_node(pre);
			pre = pre->next;
		}
		i++;
	}
}

void ft_env(t_main_node *main)
{
	t_envp_node *curr;

	curr = main->envp->head->next;
	while (curr != NULL && curr->value != NULL)
	{
		printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
}

void ft_exit(t_main_node *main)
{
	t_cmd_node *curr;
	int		exit_code;
	int		is_char;
	
	curr = main->curr;
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

    curr = main->curr;
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