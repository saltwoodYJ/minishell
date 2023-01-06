#include "minishell.h"

void	del_envp_one(t_envp_node *lst)
{
    int i;

    i = -1;
	if (lst == 0)
		return ;
    if (lst->key)
        free(lst->key);
    if (lst->value)
        free(lst->key);
	free(lst);
}

void	ft_envp_clear(t_envp_node **lst)
{
	t_envp_node	*curr;

	if (*lst == 0)
		return ;
	while (*lst != 0)
	{
		curr = (*lst)->next;
		del_envp_one(*lst);
		*lst = curr;
	}
	*lst = 0;
}
