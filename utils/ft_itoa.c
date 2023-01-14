/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_itoa.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/07/06 14:24:44 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/10 14:58:50 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <stdlib.h>

int	cnt_len(int n)
{
	int	len;

	len = 1;
	if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		n *= -1;
		len += 1;
	}
	while (n > 9)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

void	convert_minus(char *arr, int *n, int *len)
{
	arr[0] = '-';
	if ((*n) == -2147483648)
	{
		arr[10] = '8';
		(*n) = (*n) / 10;
		(*len)--;
	}
	(*n) = (*n) * -1;
}

char	*ft_itoa(int n)
{
	char	*arr;
	int		len;

	len = cnt_len(n);
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (!arr)
		return (0);
	arr[len] = '\0';
	if (n < 0)
		convert_minus(arr, &n, &len);
	while (n > 9)
	{
		arr[len - 1] = n % 10 + '0';
		n = n / 10;
		len--;
	}
	arr[len - 1] = n + '0';
	return (arr);
}
