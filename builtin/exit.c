/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:38:01 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/07 04:09:37 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	make_non_overflow(int overflow_code)
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

int	exit_with_arg(t_main_node *main, int exit_num, int is_not_num)
{
	t_cmd_node	*curr;
	int			exit_code;

	curr = main->curr;
	exit_code = exit_num;
	if (is_not_num == 1) //문자열인 경우
	{
		printf("exit\nminishell: exit: %s: numeric argument required\n", \
		curr->cmd[1]);
		exit_code = 255;
	}
	else if (curr->cmd[2]) //인자 2개 이상인 경우. exit 안함
	{
		printf("exit\nminishell: exit: too many arguments\n");
		exit_code = -1;
	}	
	else if (exit_num > 255 || exit_num < 0)
	{
		exit_code = make_non_overflow(exit_num);
		printf("exit\n");
	}
	else
		printf("exit\n");
	return (exit_code);
}

void	ft_exit(t_main_node *main)
{
	int	exit_num;
	int	exit_code;
	int	is_not_num;

 	//인자가 있을때: exit 127
	if (main->curr->cmd[1])
	{
		exit_num = ft_exit_atoi(main->curr->cmd[1], &is_not_num, 0);
		exit_code = exit_with_arg(main, exit_num, is_not_num);
		if (exit_code == -1)
		{
			main->status = 1;
			return ;
		}
		else
		{
			main->status = exit_code;
			exit(exit_code);
		}
	}
	printf("exit\n");
	main->status = 0;
	exit(0);
}