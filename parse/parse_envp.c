#include "minishell.h"

t_envp_node	*parse_envp(char **ev)
{
	t_envp_node *envp_node;
	t_envp_node	*now;
	int			index;

	envp_node = malloc(sizeof(t_envp_node));
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
	int	len;
	int	equal;	

	len = ft_strlen(envp);
	node = malloc(sizeof(t_envp_node));
	equal = (int)(ft_strchr(envp, '=') - envp); // abcd
	if (!node)
		exit (1);
	node->key = ft_substr(envp, 0, equal);
	node->value = ft_substr(envp, equal + 1, len - equal - 1);
	node->next = NULL;
	return (node);
}

char	*ft_substr(char *s, int start, int len)
{
	char	*substr;
	int 	index;
	int 	size;
	int 	srclen;

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

t_envp_node	*get_value_by_key(t_envp_node *ev_lst, char *key)
{
	t_envp_node *now;

	now = ev_lst->next;
	while (now)
	{
		if (ft_strncmp(now->key, key, ft_strlen(key) + 1) == 0)
		{
			printf("%s %s\n", now->key, now->value);
			return (now);
		}
		now = now->next;
	}
	return (NULL);
}