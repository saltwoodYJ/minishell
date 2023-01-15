/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   signal.c								   :+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: yejinam <marvin@42.fr>				 +#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2023/01/14 19:47:51 by yejinam		 #+#	#+#		   */
/*   Updated: 2023/01/15 16:21:23 by yejinam          ###   ########.fr       */
/*															 */
/* ************************************************************************** */

#include "../header/minishell.h"

void	action(int signum)
{
	if (signum == 2)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signum == 3)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	rm_ctrl(void)
{
	struct termios	attr;

	tcgetattr(STDIN_FILENO, &attr);
	attr.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &attr);
}
