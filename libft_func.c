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

int is_invalid_key(char *s, int flag)
{
	int i;

	i = 0;
	//flag export->0, unset->1
	if (!s)
		return (1);
	if (!ft_isalpha(s[i]) && s[i] != '_') //알파벳으로 시작하지도, _로 시작하지도 않는다면
		return (1);
	else if (flag == 0)
	{
		while (s[i] && s[i] != '=')
		{
			if (!ft_isdigit(s[i]) && !ft_isalpha(s[i]) && (s[i] != '_'))
				return (1);
			i++;
		}
		return (0);
	}
	else if (flag == 1)
	{
		while (s[i])
		{
			if (!ft_isdigit(s[i]) && !ft_isalpha(s[i]) && (s[i] != '_'))
				return (1);
			i++;
		}
		return (0);
	}
	return (0);
}