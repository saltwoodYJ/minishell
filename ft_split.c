/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 13:40:29 by hyeokim2          #+#    #+#             */
/*   Updated: 2022/10/11 20:50:50 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_get_len(char	*s, char c)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (s[i] != 0)
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] == 0)
			break ;
		while (s[i] != '\0' && s[i] != c)
			i++;
		len++;
	}
	return (len);
}

char	*ft_make_arr(char *s, char c, int i)
{
	char	*arr;
	int		idx;
	int		j;

	idx = 0;
	j = i;
	while (s[j] != '\0' && s[j] != c)
		j++;
	arr = (char *)malloc(sizeof(char) * (j - i + 1));
	if (!arr)
		return (0);
	while (i < j)
	{
		arr[idx] = s[i];
		idx++;
		i++;
	}
	arr[idx] = '\0';
	return (arr);
}

char	**ft_free_all(char **s, int idx)
{
	int	i;

	i = 0;
	while (i < idx)
	{
		free(s[i]);
		i++;
	}
	free(s);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		i;
	int		idx;

	arr = (char **)malloc(sizeof(char *) * (ft_get_len((char *)s, c) + 1));
	if (!arr)
		return (0);
	i = 0;
	idx = 0;
	while (s[i] != 0)
	{
		while (s[i] == c && s[i] != 0)
			i++;
		if (s[i] != c && s[i] != 0)
		{
			arr[idx] = ft_make_arr((char *)s, c, i);
			if (arr[idx] == 0)
				return (ft_free_all(arr, idx));
			idx++;
		}
		while (s[i] != c && s[i] != 0)
			i++;
	}
	arr[idx] = 0;
	return (arr);
}
