#include "minishell.h"

void	ft_lstdelone(t_cmd_node *lst)
{
    int i;

    i = 0;
	if (lst == 0)
		return ;
	if (lst->cmd)
	{
    	while (lst->cmd[i])
		{
    		free(lst->cmd[i]);
			i++;
		}
	}
	if (lst->cmd)
		free(lst->cmd);
    ft_input_clear(&(lst->infile_node));
    ft_output_clear(&(lst->outfile_node));
	free(lst);
}

void	ft_node_clear(t_cmd_node *cmd)
{
	t_cmd_node	*curr;

	curr = cm
	if (cmd == 0)
		return ;
	while (*lst != 0)
	{
		curr = (*lst)->next;
		ft_lstdelone(*lst);
		*lst = curr;
	}
	*lst = 0;
}