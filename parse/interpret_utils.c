/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   interpret_utils.c							:+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: yejinam <marvin@42.fr>				 +#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2023/01/14 18:07:39 by yejinam		 #+#	#+#		   */
/*   Updated: 2023/01/14 18:07:40 by yejinam		###   ########.fr	  */
/*															 */
/* ************************************************************************** */

char	*ft_strcat(char *str, char *value)
{
	int	i;

	if (!value)
		return ((str));
	i = 0;
	while (value[i])
	{
		*str = value[i];
		str++;
		i++;
	}
	return (str);
}

int	check_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			return (1);
		i++;
	}
	return (0);
}
