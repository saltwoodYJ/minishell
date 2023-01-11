#include "../header/minishell.h"

void	ft_interpret(t_parsing_node *parse, t_envp_node *ev_lst)
{
	t_parsing_node	*now;
	t_parsing_node	*tmp_node;
	char			*tmp;

	now = parse;
	while(now)
	{
		if (now->type != RED_H && now->next)
		{
			tmp = now->next->str;
			now->next->str = interpret(tmp, ev_lst);
			free(tmp);
			if (!now->next->str[0])
			{
				tmp_node = now->next;
				now->next = tmp_node->next;
				free(tmp_node->str);
				free(tmp_node);
			}
		}
		now = now->next;
	}
}

char	*interpret(char *str, t_envp_node *ev_lst)
{
	t_envp_node	*node;
	int key;
	int	i;
	char *tmp;

	char *new_str = new_red_node(sizeof(char) * (get_len_ev(str, ev_lst) + 1));
	tmp = new_str;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && is_quote(str, i) != 2)
		{
			key = 0;
			while (str[i + key] && str[i + key] != ' ' && str[i + key]!='"' && str[i + key] != '\'')
				key++;
			node = get_value_by_key(ev_lst, &str[i + 1], key);
			if (node)
				new_str = ft_strcat(new_str, node->value);
			i += key;
		}
		else
		{
			if (str[i] != '\"')
				*new_str = str[i];
				new_str++;
			i++;
		}
	}
	*new_str = 0;
	return (tmp);
}

char	*ft_strcat(char *str, char *value)
{
	int	i;

	i = 0;
	while(value[i])
	{
		*str = value[i];
		str++;
		i++; 
	}
	return (str);
}

int	get_len_ev(char *str, t_envp_node *ev_lst)
{
	t_envp_node	*value;
	int len;
	int	i;
	int	key;

	len = ft_strlen(str);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && is_quote(str, i) != 2)
		{
			key = 0;
			while (str[i + key] && str[i + key] != ' ')
				key++;
			if (key)
				value = get_value_by_key(ev_lst, &str[i + 1], key);
			if (value)
				len += ft_strlen(value->value);
			i += key;
		}
		else
			i++;
	}
	return (len);
}

int	check_dollar(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		if (str[i] == '$' && str[i + 1])
			return (1);
		i++;
	}
	return (0);
}