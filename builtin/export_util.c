/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:00:56 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/10 21:17:26 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	search_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/* flag export->0, unset->1 */
int	is_invalid_key(char *s, int flag)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	if (!ft_isalpha(s[0]) && s[0] != '_') //알파벳으로 시작하지도, _로 시작하지도 않는다면
		return (1);
	if (flag == 0)
	{
		while (s[i] && s[i] != '=')
		{
			if (!ft_isdigit(s[i]) && !ft_isalpha(s[i]) && (s[i] != '_'))
				return (1);
			i++;
		}
		return (0);
	}
	while (s[i])
	{
		if (!ft_isdigit(s[i]) && !ft_isalpha(s[i]) && (s[i] != '_'))
			return (1);
		i++;
	}
	return (0);
}

char	*get_env_path(t_envp_node *envp, char *key)
{
	t_envp_node	*curr;

	curr = envp->next->next;
	while (curr != NULL)
	{
		if (!ft_strcmp(curr->key, key))
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}

char	**make_key_value(char *cmd)
{
	char	**arr;
	int 	idx;
	int		i;
	int		j;

	if (!cmd)
		return (0);
	idx = search_equal(cmd);
	arr = (char **)malloc(sizeof(char *) * 3);
	if (!arr)
		return (0);
	arr[0] = (char *)malloc(idx + 1);
	arr[1] = (char *)malloc(ft_strlen(cmd) - idx + 1);
	if (!arr[0] || !arr[1])
		return (0);
	i = 0;
	j = 0;
	while (i < idx)
		arr[0][i++] = cmd[j++];
	arr[0][i] = '\0';
	i = 0;
	j = idx + 1;
	while (j < ft_strlen(cmd))
		arr[1][i++] = cmd[j++];
	arr[1][i] = '\0';
	arr[2] = 0;
	return (arr);
}
