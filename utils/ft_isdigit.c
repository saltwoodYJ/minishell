/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_isdigit.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hyeokim2 <hyeokim2@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/07/13 13:36:26 by hyeokim2		  #+#	#+#			 */
/*   Updated: 2023/01/06 23:35:08 by hyeokim2		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
