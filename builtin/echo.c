/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:38:30 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/09 17:13:44 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_option(char *str)
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

void	ft_echo(t_main_node *main)
{
	char	**cmd;
	int		i;
	int		nl;

	cmd = main->curr->cmd;
	i = 1;
	nl = 1;
	if (is_n_option(cmd[1]))
	{
		nl = 0;
		i++;
	}
	while (is_n_option(cmd[i]))
		i++;
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
	if (nl == 1)
		printf("\n");
}
