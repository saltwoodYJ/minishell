#include "minishell.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	return (0);
}

int is_valid_key(char *s)
{
	int i;

	i = 0;
	if (!s)
		return (0);
	if (ft_isalpha(s[i]) || s[i] == '_')
	{
		while (s[i])
		{
			if (!ft_isdigit(s[i]) && !ft_isalpha(s[i]) && (s[i] != '_'))
				return (0);
			i++;
		}
	}
	return (1);
}