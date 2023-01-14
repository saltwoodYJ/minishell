/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   syntax_err.c								:+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: yejinam <marvin@42.fr>				 +#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2023/01/14 15:45:30 by yejinam		 #+#	#+#		   */
/*   Updated: 2023/01/14 15:45:35 by yejinam		###   ########.fr	  */
/*															 */
/* ************************************************************************** */

#include "../header/minishell.h"

int	print_syntax_err(const char *str)
{
	printf("minishell: syntax error near unexpected token `%s\'\n", str);
	return (1);
}

int	check_pipe_err(t_parsing_node *parse, t_parsing_node *now)
{
	int	ret;

	ret = 0;
	if (!now->next)
	{
		if (now != parse)
			ret = print_syntax_err("newline");
	}
	else if (now->next->type == PIPE)
		ret = print_syntax_err(now->next->str);
	return (ret);
}

int	check_red_err(t_parsing_node *now)
{
	int	ret;

	ret = 0;
	if (!now->next)
		ret = print_syntax_err("newline");
	else if (now->next->type != WORD)
		ret = print_syntax_err(now->next->str);
	return (ret);
}

int	check_quote_err(char *line)
{
	int	quote;
	int	ret;

	ret = 0;
	quote = is_quote(line, ft_strlen(line) - 1);
	if (quote == 1)
		ret = print_syntax_err("\"");
	if (quote == 2)
		ret = print_syntax_err("\'");
	return (ret);
}
