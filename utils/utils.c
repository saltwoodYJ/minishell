/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:36:36 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/12 22:13:10 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_comment(char *s1, char *s2)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin3("minishell: ", s1, ": ");
	temp2 = ft_strjoin(temp, s2);
	free(temp);
	perror(temp2);
	free(temp2);
}

void	ft_putstr_err(char *cmd, char *arg, char *comment)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(comment, 2);
	ft_putstr_fd("\n", 2);
}

int	error_msg(t_main_node *main, char *arg, t_error error, int status)
{
	dup2(main->stdout_fd, 1);
	if (error == FILE_ERROR)
	{
		// printf("minishell: %s: No such file or directory\n", arg);
		ft_putstr_err(NULL, arg, "No such file or directory");
	}
	if (error == CMD_ERROR)
	{
		// printf("minishell: %s: command not found\n", arg);
		ft_putstr_err(NULL, arg, "command not found");
	}
	else if (error == EXPORT_KEY_ERROR)
	{
		// printf("minishell: export: `%s': not a valid identifier\n", arg);
		ft_putstr_err("export", arg, "not a valid identifier");
	}
	else if (error == UNSET_KEY_ERROR)
	{
		ft_putstr_err("unset", arg, "not a valid identifier");
	}
	else if (error == NOT_SET_ERROR)
	{
		// printf("minishell: cd: %s not set\n", arg);
		ft_putstr_err("cd", arg, "not set");
	}
	else if (error == NUM_ARG_ERROR)
	{
		// printf("minishell: exit: %s: numeric argument required\n", arg);
		ft_putstr_err("exit", arg, "numeric argument required");
	}
	else if (error == MANY_ARG_ERROR)
	{
		ft_putstr_err("exit", NULL, "too many arguments");
		// printf("minishell: exit: too many arguments\n");
	}
	else if (error == EXEC_ERROR)
	{
		// printf("minishell: %s: %s exec error\n", arg, arg);
		ft_putstr_err(arg, arg, "exec error");
	}
	main->status = status;
	return (status);
}

void	ft_free_str(char **s1, char **s2)
{
	int	i;

	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			free(s1[i]);
			i++;
		}
		free(s1);
		s1 = 0;
	}
	else
	{
		free(*s2);
		*s2 = 0;
	}
}

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = (char *)malloc(sizeof(char) * \
	(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1));
	if (!str)
		return (0);
	while (s1 && s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2 && s2[i])
		str[j++] = s2[i++];
	i = 0;
	while (s3 && s3[i])
		str[j++] = s3[i++];
	str[j] = '\0';
	return (str);
}

size_t	ft_double_strlen(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}
