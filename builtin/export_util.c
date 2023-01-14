/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   export_util.c							   :+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: hyeokim2 <hyeokim2@student.42.fr>		+#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2023/01/07 00:00:56 by hyeokim2		#+#	#+#		   */
/*   Updated: 2023/01/14 19:37:42 by hyeokim2		###   ########.fr	  */
/*															 */
/* ************************************************************************** */

#include "minishell.h"

int	ft_search_char(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
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
	if (!ft_isalpha(s[0]) && s[0] != '_')
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

char	*make_arg(char *cmd, int start, int end)
{
	char	*arr;
	int		i;

	i = 0;
	arr = (char *)malloc(end - start + 1);
	if (!arr)
		return (0);
	while (start < end)
	{
		arr[i] = cmd[start];
		start++;
		i++;
	}
	arr[i] = '\0';
	return (arr);
}

char	**make_key_value(char *cmd)
{
	char	**args;
	int		idx;

	if (!cmd)
		return (0);
	idx = ft_search_char(cmd, '=');
	args = (char **)malloc(sizeof(char *) * 3);
	if (!args)
		return (0);
	args[0] = make_arg(cmd, 0, idx);
	if (!args[0])
		free(args);
	args[1] = make_arg(cmd, idx + 1, ft_strlen(cmd));
	if (!args[1])
	{
		free(args[0]);
		free(args);
	}
	args[2] = 0;
	return (args);
}
