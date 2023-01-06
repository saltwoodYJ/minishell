#include "minishell.h"

void perror_comment(char *s1, char *s2)
{
	char *temp;
	char *temp2;

	temp = ft_strjoin3("minishell: ", s1, ": ");
	temp2 = ft_strjoin(temp, s2);
	free(temp);
	perror(temp2);
	free(temp2);
}

int is_n_option(char *str)
{
	int	i;

	i = 0;
	if (ft_strcmp(str, "-n") == 0)
		return (1);
	if (str[i] == '-')
	{
		i++;
		while (str[i])
		{
			if (str[i] != 'n')
				return (0);
			i++;
		}
		return (1);
	}
	else
		return (0);
}

void ft_echo(t_main_node *main)
{
	char **cmd;
	int		i;
	int		nl;

	cmd = main->curr->cmd; /* echo의 다음 인자 */
	i = 1;
	nl = ON;
	if (is_n_option(cmd[1]))
	{
		nl = OFF;
		i++;
	}
	if (cmd[i])
	{
		printf("%s", cmd[i]);
		i++;
	}
	while (cmd[i])
	{
		printf(" %s", cmd[i]);
		i++;
	}
	if (nl == ON)
		printf("\n");
}

char *get_env_path(t_envp_node *envp, char *key)
{
	t_envp_node *curr;

	curr = envp->next->next;
	while (curr != NULL)
	{
		if (!ft_strcmp(curr->key, key))
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}


void	show_envp(t_main_node *main)
{
	t_envp_node *curr;

	curr = main->ev_lst->next;
	while (curr)
	{
		printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
}


void ft_cd(t_main_node *main)
{
	char	*path;
	char	*old_path;
	char	**cmd;

	cmd = main->curr->cmd;
	old_path = getcwd(NULL, 0);
	// show_cmd(main);
	if (!cmd[1] || !ft_strcmp(cmd[1], "~")) /* cd만 있을경우 */
	{
		path = get_env_path(main->ev_lst, "HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set");
			main->status = 1;
			free(old_path);
			return ;
		}
	}
	else if (!ft_strcmp(cmd[1], "-"))
	{
		path = get_env_path(main->ev_lst, "OLDPWD"); //OLDPWD가 없으면?
		printf("%s\n", path);
	}
	else
		path = main->curr->cmd[1]; //현재 위치에서 구해야
	if (chdir(path) == 0) //성공
	{
		path = getcwd(NULL, 0);
		add_env(main, "PWD", path);
		add_env(main, "OLDPWD", old_path);
		main->status = 0;
	}
	else
	{
		perror_comment("cd", cmd[1]);
		main->status = 1;
		free(path);
		free(old_path);
	}
}

void ft_pwd(t_main_node *main)
{
	char	*path;
	
	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		printf("minishell: pwd: pwd exec error\n");
		main->status = 1;
		return ;
	}
	printf("%s\n", path);
	main->status = 0;
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
	t_envp_node *pre;
	//value가 없으면 NULL로 들어감
	//value 있어야하나 값이 없으면 '\0'로]
	curr = main->ev_lst->next;
	pre = main->ev_lst;
	while (curr)
	{
		//변수에 숫자없는지 확인
		if (!ft_strcmp(curr->key, key)) /* 이미 있는 키값 */
		{
			if (!value)
				return ;
			curr->value = value; //value는 나중에 free해버리니까 복사해야함
			return ;
		}
		curr = curr->next;
		pre = pre->next;
	}
	pre->next = make_envp_node(key, value);
	main->status = 0;
}

void show_export(t_main_node *main)
{
	t_envp_node *curr;

	curr = main->ev_lst->next;
	while (curr != NULL)
	{
		if (!curr->value)
			printf("declare -x %s\n", curr->key);
		else
			printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
		curr = curr->next;
	}
	main->status = 0;
}

int	search_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (i);
		i++;
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
		//=으로 시작할때, key가 _과 문자열로 시작하지 않을 때 에러
		if (search_equal(cmd[i]) == 0 || is_invalid_key(cmd[i], 0)) 
		{
			printf("minishell: export: `%s': not a valid identifier\n", cmd[i]);
			main->status = 1;
		}
		else if (search_equal(cmd[i]) == -1) //=이 없음
		{
			add_env(main, cmd[i], NULL);
			main->status = 0;
		}	
		else
		{
			args = ft_split(cmd[i], '='); //=기준으로 key, value 나누기
			if (args[1] == NULL)
				args[1] = ft_strdup("\0"); 
			add_env(main, args[0], args[1]);
			main->status = 0;
		}
		i++;
	}

	//만약 value가 여러개 오면 뒤에껀 무시
	//만약 =은 있고 value가 없다면 없는 채로 들어감
	//=도 없고 value도 없다면 그냥 export에만 들어감
	// 1. =은 있지만 value가 없는 경우 -> value와 key 모두 저장 key는 "\0"이 들어감
	// 2-1. =도 없고 (이건 맨처음 검사) value도 없는 경우 -> 그냥 value없이 key만 저장
	// 2-2. export 뒤에 = 없이 여러개 들어오면 여러개가 그냥 선언됨.
}

void delete_envp_node(t_envp_node *pre)
{
	t_envp_node *old;

	old = pre->next;
	if (old == NULL)
		return ;
	pre->next = old->next;
	// free(old->key);
	// free(old->value);
	// free(old);
}

void ft_unset(t_main_node *main) //에러처리 필요
{
	t_envp_node *pre;
	char		**cmd;
	int			i;

	pre = main->ev_lst->next;
	cmd = main->curr->cmd;
	i = 1;
	while (cmd[i])
	{
		if (is_invalid_key(cmd[i], 1))
			printf("minishell: unset: `%s': not a valid identifier\n", cmd[i]);
		else
		{
			pre = main->ev_lst->next;
			while (pre->next != NULL)
			{
				if (!ft_strcmp(pre->next->key, cmd[i]))
					delete_envp_node(pre);
				else
					pre = pre->next;
			}
		}
		i++;
	}
}

void ft_env(t_main_node *main)
{
	t_envp_node *curr;

	curr = main->ev_lst->next;
	while (curr != NULL && curr->value != NULL)
	{
		printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
	main->status = 0;
}

int	make_exit_code(int overflow_code)
{
	if (overflow_code > 0)
	{
		while (overflow_code > 0)
			overflow_code = overflow_code - 256;
	}
	else
	{
		while (overflow_code < 0)
			overflow_code = overflow_code + 256;
	}
	return (overflow_code);
}

void ft_exit(t_main_node *main)
{
	t_cmd_node *curr;
	int		exit_code;
	int		is_not_num;
	
	curr = main->curr;
	/*종료 코드 저장!! */
	if (curr->cmd[1])  //인자가 있을때: exit 127
	{
		exit_code = ft_exit_atoi(curr->cmd[1], &is_not_num);
		if (is_not_num == 1) //문자열인 경우
		{
			printf("exit\nminishell: exit: %s: numeric argument required\n", curr->cmd[1]);
			exit_code = 255;
		}
		else if (curr->cmd[2]) //인자 2개 이상인 경우
		{
			printf("exit\nminishell: exit: too many arguments\n");
			main->status = 1; //exit안함
			return ;
		}
		else if (exit_code > 255 || exit_code < 0) //인자가 0~255 범위를 넘어갈경우
		{
			exit_code = make_exit_code(exit_code);
			printf("exit\n");
		}
		else
			printf("exit\n");
		main->status = exit_code;
		exit(exit_code);
	}
	//인자가 없을때: exit;
	printf("exit\n");
	main->status = 0;
	exit(0);
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
		ft_pwd(main);
	else if (ft_strcmp(curr->cmd[0], "export") == 0)
		ft_export(main);
	else if (ft_strcmp(curr->cmd[0], "unset") == 0)
		ft_unset(main);
	else if (ft_strcmp(curr->cmd[0], "env") == 0)
		ft_env(main);
	else if (ft_strcmp(curr->cmd[0], "exit") == 0)
		ft_exit(main);
}