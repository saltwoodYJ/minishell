/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_atoi.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/13 21:19:05 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/13 21:19:12 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	long long	num;
	int			sign;

	num = 0;
	sign = 1;
	while (*str == '\t' || *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r' || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while ('0' <= *str && *str <= '9')
	{
		if (num > 922337203685477580)
			return ((sign + 1) / -2);
		num *= 10;
		if (num > 9223372036854775807 - (*str - '0'))
			return ((sign - 1) / -2);
		num += *str - '0';
		str++;
	}
	return (num * sign);
}
