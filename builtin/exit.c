/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   exit.c									:+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: hyeokim2 <hyeokim2@student.42.fr>		+#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2023/01/06 23:38:01 by hyeokim2		#+#	#+#		   */
/*   Updated: 2023/01/14 19:35:51 by hyeokim2		###   ########.fr	  */
/*															 */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isdigit_str(char *str, int i)
{
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit_atoi(char *str, int *is_not_num, int i)
{
	long long	num;
	int			sign;

	sign = 1;
	num = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	if (!str[i] || !ft_isdigit_str(str, i))
	{
		*is_not_num = 1;
		return (-1);
	}
	while (str[i] >= '0' && str[i] <= '9')
	{	
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num * sign);
}

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

int	exit_with_arg(t_main_node *main, int exit_arg, int is_not_num)
{
	int	exit_code;

	exit_code = exit_arg;
	if (is_not_num == 1)
	{
		error_msg(main, main->curr->cmd[1], NUM_ARG_ERROR, 255);
		exit_code = 255;
	}
	else if (main->curr->cmd[2])
	{
		error_msg(main, NULL, MANY_ARG_ERROR, 1);
		return (-1);
	}
	else if (exit_arg > 255 || exit_arg < 0)
		exit_code = make_non_overflow(exit_arg);
	return (exit_code);
}

void	ft_exit(t_main_node *main)
{
	int	exit_arg;
	int	exit_code;
	int	is_not_num;

	is_not_num = 0;
	if (main->cmd_num == 1)
		printf("exit\n");
	if (main->curr->cmd[1])
	{
		exit_arg = ft_exit_atoi(main->curr->cmd[1], &is_not_num, 0);
		exit_code = exit_with_arg(main, exit_arg, is_not_num);
		if (exit_code == -1)
		{
			main->status = 1;
			return ;
		}
		main->status = exit_code;
		exit(exit_code);
	}
	if (main->cmd_num == 1)
		exit(main->status);
	main->status = 0;
	exit(0);
}
