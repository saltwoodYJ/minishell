#include "minishell.h"

void	del_input_one(t_infile_node *lst)
{
    int i;

    i = -1;
	if (lst == 0)
		return ;
    if (lst->file)
        free(lst->file);
    if (lst->limiter)
        free(lst->limiter);
	free(lst);
}

void	ft_input_clear(t_infile_node **lst)
{
	t_infile_node	*curr;

	if (*lst == 0)
		return ;
	while (*lst != 0)
	{
		curr = (*lst)->next;
		del_input_one(*lst);
		*lst = curr;
	}
	*lst = 0;
}

void	del_output_one(t_outfile_node *lst)
{
    int i;

    i = -1;
	if (lst == 0)
		return ;
    if (lst->file)
        free(lst->file);
	free(lst);
}

void	ft_output_clear(t_outfile_node **lst)
{
	t_outfile_node	*curr;

	if (*lst == 0)
		return ;
	while (*lst != 0)
	{
		curr = (*lst)->next;
		del_output_one(*lst);
		*lst = curr;
	}
	*lst = 0;
}
