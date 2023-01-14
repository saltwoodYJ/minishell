/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   interpret.c								:+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: yejinam <marvin@42.fr>				 +#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2023/01/14 16:14:58 by yejinam		 #+#	#+#		   */
/*   Updated: 2023/01/14 22:40:29 by yejinam          ###   ########.fr       */
/*															 */
/* ************************************************************************** */

#include "../header/minishell.h"

void	ft_interpret(t_parsing_node *parse, t_envp_node *ev_lst, int status)
{
	t_parsing_node	*now;
	char			*tmp;

	now = parse;
	while (now)
	{
		if (now->type != RED_H && now->next)
		{
			tmp = now->next->str;
			now->next->str = interpret(tmp, ev_lst, status);
			ft_free(tmp);
		}
		if (now->next)
		{
			if (now->next->str[0])
				rm_quote(now->next);
			else
				now->next->str = ft_free(now->next->str);
		}
		now = now->next;
	}
}

void	rm_quote(t_parsing_node *node)
{
	char	*tmp;
	char	*str;
	int		i_str;
	int		i_tmp;

	str = node->str;
	tmp = ft_malloc(sizeof(char) * (ft_strlen(node->str) + 1));
	i_str = 0;
	i_tmp = 0;
	while (str[i_str])
	{
		if (!(str[i_str] == '"' && is_quote(str, i_str) != 2)
			&& !(str[i_str] == '\'' && is_quote(str, i_str) != 1))
		{
			tmp[i_tmp] = str[i_str];
			i_tmp++;
		}
		i_str++;
	}
	node->str = tmp;
	ft_free(str);
}

char	*interpret(char *str, t_envp_node *ev_lst, int status)
{
	char	*tmp;
	char	*new_str;
	char	*value;
	int		i;

	new_str = ft_malloc(sizeof(char) * (get_len_ev(str, ev_lst, status) + 1));
	tmp = new_str;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && is_quote(str, i) != 2 && str[i + 1]
			&& (ft_isalpha(str[i + 1]) || ft_isdigit(str[i + 1])
				|| str[i + 1] == '?'))
		{
			i++;
			i += key_to_value(&str[i], ev_lst, &value, status);
			new_str = ft_strcat(new_str, value);
			if (str[i - 1] == '?')
				free(value);
		}
		else
			*new_str++ = str[i++];
	}
	*new_str = 0;
	return (tmp);
}

int	get_len_ev(char *str, t_envp_node *ev_lst, int status)
{
	char	*value;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && is_quote(str, i) != 2 && str[i + 1]
			&& (ft_isalpha(str[i + 1]) || ft_isdigit(str[i + 1])
				|| str[i + 1] == '?'))
		{
			i++;
			i += key_to_value(&str[i], ev_lst, &value, status);
			len += ft_strlen(value);
			if (str[i - 1] == '?')
				ft_free(value);
		}
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}

int	key_to_value(char *str, t_envp_node *ev_lst, char **value, int status)
{
	int	key;

	key = 0;
	while (str[key] && (ft_isalpha(str[key]) || ft_isdigit(str[key])))
		key++;
	*value = get_value_by_key(ev_lst, str, key);
	if (key == 0 && str[0] == '?')
	{
		*value = ft_itoa(status);
		key = 1;
	}
	return (key);
}
