
#include "minishell.h"

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


void exec_non_builtin(t_main_node *main)
{
	char 	**cmd_args;
	char	*path;

	/* cmd 합치기 */
	// cmd_args = search_cmd(cmd);
	// if (!cmd)
	// 	return ; //오류 처리
	cmd_args = main->node_head->cmd;
	path = get_path(main->ev, cmd_args[0]);
	if (!path)
	{
		printf("minishell: command not found: %s\n", cmd_args[0]);
		main->status = 127;
		ft_free(cmd_args, 0); /* 문제!! */
		exit(127);
	}
	execve(path, cmd_args, main->ev);
	// dup2(main->output_fd, 1);
	perror("minishell :");
	if (errno == ENOEXEC) //errno 처리
        exit(126);
    else if(errno == ENOENT) //errno 처리
        exit(127);
    else
        exit(EXIT_FAILURE);
}
