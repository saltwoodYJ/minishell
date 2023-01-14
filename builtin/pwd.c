/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   pwd.c											  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/06 23:38:24 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/13 22:13:46 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_main_node *main)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("minishell: pwd");
		main->status = 1;
		return ;
	}
	printf("%s\n", path);
	main->status = 0;
	free(path);
}
