/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   parse_envp.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/07 00:09:31 by yejinam		   #+#	#+#			 */
/*   Updated: 2023/01/12 20:32:26 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../header/minishell.h"

t_envp_node	*parse_envp(char **ev)
{
	t_envp_node	*envp_node;
	t_envp_node	*now;
	int			index;

	envp_node = ft_malloc(sizeof(t_envp_node));
	now = envp_node;
	index = 0;
	while (ev[index])
	{
		now->next = new_envp_node(ev[index]);
		now = now->next;
		index++;
	}
	return (envp_node);
}

t_envp_node	*new_envp_node(char *envp)
{
	t_envp_node	*node;
	int			len;
	int			equal;	

	len = ft_strlen(envp);
	node = malloc(sizeof(t_envp_node));
	equal = (int)(ft_strchr(envp, '=') - envp);
	if (!node)
		exit (1);
	node->key = ft_substr(envp, 0, equal);
	node->value = ft_substr(envp, equal + 1, len - equal - 1);
	if (ft_strncmp(node->key, "OLDPWD", 6) == 0)
	{
		free(node->value);
		node->value = NULL;
	}
	node->next = NULL;
	return (node);
}

char	*ft_substr(char *s, int start, int len)
{
	char	*substr;
	int		index;
	int		size;
	int		srclen;

	srclen = ft_strlen(s);
	if (srclen <= start)
		size = 0;
	else if (srclen < len + start)
		size = srclen - start;
	else
		size = len;
	substr = (char *)malloc(sizeof(char) * (size + 1));
	if (!substr)
		exit (1);
	index = 0;
	while (index < size && s[start])
		substr[index++] = s[start++];
	substr[index] = 0;
	return (substr);
}

char	*get_value_by_key(t_envp_node *ev_lst, char *key, int len)
{
	t_envp_node	*now;

	if (len == 0)
		return (NULL);
	now = ev_lst->next;
	while (now)
	{
		if (ft_strncmp(now->key, key, len) == 0 && !now->key[len])
			return (now->value);
		now = now->next;
	}
	return (NULL);
}
