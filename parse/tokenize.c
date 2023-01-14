/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   tokenize.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: yejinam <marvin@42.fr>					 +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/14 14:33:22 by yejinam		   #+#	#+#			 */
/*   Updated: 2023/01/14 14:33:26 by yejinam		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

int	make_token(char *line, t_main_node *main)
{
	t_parsing_node	*parse;

	ft_parse(line, &parse);
	ft_interpret(parse, main->ev_lst, main->status);
	make_cmd_list(parse, main);
	main->parse = parse;
	return (1);
}

char	*ft_worddup(char *str, int index, int len)
{
	char	*worddup;
	int		i;

	i = 0;
	worddup = (char *)malloc(sizeof(char) * (len + 1));
	if (!worddup)
		exit (1);
	while (i < len)
	{
		worddup[i] = str[index];
		i++;
		index++;
	}
	worddup[i] = 0;
	return (worddup);
}

void	ft_parse(char *s, t_parsing_node **parse)
{
	t_parsing_node	*now;
	int				len;
	int				index;

	*parse = ft_malloc(sizeof(t_parsing_node));
	if (!*parse)
		exit (1);
	now = *parse;
	(*parse)->type = PIPE;
	index = 0;
	while (s[index])
	{
		len = 0;
		if (is_sep(s, index + len) && s[index])
			make_sep(s, &index, &len);
		else
			while ((is_sep(s, index + len) == 0 || is_quote(s, index + len))
				&& s[index + len])
		len++;
		if (len)
		{
			add_parsing_node(&now, ft_worddup(s, index, len));
			index += len;
		}
	}
}

int	add_parsing_node(t_parsing_node **now, char *str)
{
	t_parsing_node	*node;

	node = malloc(sizeof(t_parsing_node));
	if (!node)
		exit (1);
	(*now)->next = node;
	node->str = str;
	node->type = get_type(str);
	node->next = NULL;
	*now = node;
	return (1);
}
/*
1. 분리 후 토큰화
	1.1 공백 분리 -> 노드 안만듬
	2.2 깜찍이 분리 -> 노드 만듬
	3.3 파이프 분리 -> 노드 만듬
	4.4 '' "" 분리 알아서 하세요

상태정리 -> argv, i/o red, sq/dq(나중에 생각), pipe
*/
