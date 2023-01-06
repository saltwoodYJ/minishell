/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:00:56 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/07 03:23:53 by hyeokim2         ###   ########.fr       */
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
