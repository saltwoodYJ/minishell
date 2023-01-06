/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:36:36 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/07 04:09:13 by hyeokim2         ###   ########.fr       */
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

void	ft_free(char **s1, char **s2)
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
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	i = 0;
	while (s3[i])
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

int	ft_exit_atoi(const char *str, int *is_not_num, int i)
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
	while (str[i] >= '0' && str[i] <= '9')
	{	
		num = num * 10 + (str[i] - '0');
		i++;
	}
	if (str[i])
	{
		*is_not_num = 1;
		return (-1);
	}
	*is_not_num = 0;
	return (num * sign);
}
