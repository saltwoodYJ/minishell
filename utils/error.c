/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   error.c									:+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: hyeokim2 <hyeokim2@student.42.fr>		+#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2023/01/13 17:18:17 by hyeokim2		#+#	#+#		   */
/*   Updated: 2023/01/14 19:42:52 by hyeokim2		###   ########.fr	  */
/*															 */
/* ************************************************************************** */

#include "minishell.h"

int	perror_comment(t_main_node *main, char *s1, char *s2, int status)
{
	char	*temp;
	char	*temp2;

	if (s1)
	{
		temp = ft_strjoin3("minishell: ", s1, ": ");
		temp2 = ft_strjoin(temp, s2);
		free(temp);
		perror(temp2);
		free(temp2);
	}
	else
	{
		temp = ft_strjoin("minishell: ", s2);
		perror(temp);
		free(temp);
	}
	main->status = status;
	return (status);
}

void	ft_putstr_err(char *cmd, char *arg, char *comment, t_error error)
{
	ft_putstr_fd("minishell", 2);
	if (cmd)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(cmd, 2);
	}
	if (arg)
	{
		ft_putstr_fd(": ", 2);
		if (error == EXPORT_KEY_ERROR || error == UNSET_KEY_ERROR)
		{
			ft_putstr_fd("`", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("'", 2);
		}
		else if (error == NOT_SET_ERROR)
			ft_putstr_fd(arg, 2);
		else
			ft_putstr_fd(arg, 2);
	}
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(comment, 2);
	ft_putstr_fd("\n", 2);
}

int	error_msg(t_main_node *main, char *arg, t_error error, int status)
{
	if (error == FILE_ERROR)
		ft_putstr_err(NULL, arg, "No such file or directory", FILE_ERROR);
	if (error == CMD_ERROR)
		ft_putstr_err(NULL, arg, "command not found", CMD_ERROR);
	else if (error == EXPORT_KEY_ERROR)
		ft_putstr_err("export", arg, "not a valid identifier", \
		EXPORT_KEY_ERROR);
	else if (error == UNSET_KEY_ERROR)
		ft_putstr_err("unset", arg, "not a valid identifier", UNSET_KEY_ERROR);
	else if (error == NOT_SET_ERROR)
		ft_putstr_err("cd", arg, "not set", NOT_SET_ERROR);
	else if (error == NUM_ARG_ERROR)
		ft_putstr_err("exit", arg, "numeric argument required", NUM_ARG_ERROR);
	else if (error == MANY_ARG_ERROR)
		ft_putstr_err("exit", NULL, "too many arguments", MANY_ARG_ERROR);
	else if (error == EXEC_ERROR)
		ft_putstr_err(arg, arg, "exec error", EXEC_ERROR);
	else if (error == PERMISSION_ERROR)
		ft_putstr_err(NULL, arg, "Permission denied", PERMISSION_ERROR);
	main->status = status;
	return (status);
}
