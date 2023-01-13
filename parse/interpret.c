#include "../header/minishell.h"

void	ft_interpret(t_parsing_node *parse, t_envp_node *ev_lst, int status)
{
	t_parsing_node	*now;
	char			*tmp;

	now = parse;
	while(now)
	{
		if (now->type != RED_H && now->next)
		{
			tmp = now->next->str;
			now->next->str = interpret(tmp, ev_lst, status);
			ft_free(tmp);
		}
		now = now->next;
	}
}

char	*interpret(char *str, t_envp_node *ev_lst, int status)
{
	char	*value; 
	int key;
	int	i;
	char *tmp;

	char *new_str = new_red_node(sizeof(char) * (get_len_ev(str, ev_lst, status) + 1));
	tmp = new_str;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1]) || ft_isdigit(str[i + 1]) || str[i + 1] == '?') && is_quote(str, i) != 2)
		{
			i++;
			key = 0;
			while (str[i + key] && (ft_isalpha(str[i + key]) || ft_isdigit(str[i + key])))
				key++;
			value = get_value_by_key(ev_lst, &str[i], key);
			if (key == 0 && str[i] == '?')
			{
				key++;
				value = ft_itoa(status);
			}
			new_str = ft_strcat(new_str, value);
			i += key;
		}
		else
		{
			if (!(str[i] == '"' && is_quote(str, i) != 2) && !(str[i] =='\'' && is_quote(str, i) != 1))
			{
				*new_str = str[i];
				new_str++;
			}
			i++;
		}
	}
	*new_str = 0;
	return (tmp);
}

char	*ft_strcat(char *str, char *value)
{
	int	i;

	if (!value)
		return(str);
	i = 0;
	while(value[i])
	{
		*str = value[i];
		str++;
		i++; 
	}
	return (str);
}

int	get_len_ev(char *str, t_envp_node *ev_lst, int status)
{
	char	*value;
	int len;
	int	i;
	int	key;

	value = NULL;
	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1]) || ft_isdigit(str[i + 1]) || str[i + 1] == '?') && is_quote(str, i) != 2)
		{
			i++;
			key = 0; // "$hello 'yeji'"
			while (str[i + key] && (ft_isalpha(str[i + key]) || ft_isdigit(str[i + key])))
				key++;
			value = get_value_by_key(ev_lst, &str[i], key);
			if (key == 0 && str[i] == '?')
			{
				value = ft_itoa(status);
				key ++;
			}
			len += ft_strlen(value);
			i += key;
		}
		else
		{
			i++;
			len++;
		}
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