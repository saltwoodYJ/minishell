/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:38:24 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/11 17:39:22 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_main_node *main)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("minishell: pwd");
		return ;
	}
	printf("%s\n", path);
	main->status = 0;
	free(path);
}
