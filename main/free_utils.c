/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   free_main.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/07 05:18:52 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/11 13:47:47 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

void	*ft_free(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

void	free_one_node(t_cmd_node *node)
{
	int	i;

	i = 0;
	if (node == 0)
		return ;
	if (node->cmd)
	{
		while (node->cmd[i])
		{
			ft_free(node->cmd[i]);
			i++;
		}
		ft_free(node->cmd);
	}
	ft_free(node);
}

void	free_double_char(char **cmd)
{
	int		i;
	char	*str;

	i = 0;
	if (!cmd)
		return ;
	str = cmd[i];
	while (cmd[i])
	{
		printf("free %p %s\n", cmd[i], cmd[i]);
		ft_free(cmd[i]);
		i++;
	}
	ft_free(cmd);
}
