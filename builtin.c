#include "minishell.h"

void ft_echo(t_data *data)
{

}

void ft_cd(t_data *data)
{
	
}

void ft_pwd(t_data *data)
{
	char	*curr_dir;
	
	curr_dir = getcwd(NULL, 0);
	if (curr_dir == NULL)
	{
		printf("Error: getcwd() cannot execute\n");
		exit(1);
	}
	printf("%s\n", curr_dir);
	free(curr_dir);
}

void ft_export(t_data *data)
{
	t_node *temp;

	temp = data->curr;
	

}

void ft_unset(t_data *data)
{

}

void ft_env(t_data *data)
{
	int i;

	i = 0;
	while (data->envp[i])
	{
		printf("%s\n", data->envp[i]);
		i++;
	}
}

void ft_exit2(t_data *data)
{

}

void exec_builtin(t_data *data)
{
    t_node *temp;

    temp = data->curr;
    if (ft_strncmp(temp->str, "echo", 5) == 0)
		ft_echo(data);
	else if (ft_strncmp(temp->str, "cd", 3) == 0)
		ft_cd(data);
	else if (ft_strncmp(temp->str, "pwd", 4) == 0)
		ft_pwd(data);
	else if (ft_strncmp(temp->str, "export", 7) == 0)
		ft_export(data);
	else if (ft_strncmp(temp->str, "unset", 6) == 0)
		ft_unset(data);
	else if (ft_strncmp(temp->str, "env", 4) == 0)
		ft_env(data);
	else if (ft_strncmp(temp->str, "exit", 5) == 0)
		ft_exit2(data);
}