#include "../minishell.h"

void	set_red(t_parsing_node *parse, t_cmd_node *node)
{
	t_parsing_node *p_now;

	p_now = parse->next;
	while (p_now && p_now->type != PIPE)
	{
		if (p_now->type == RED_A || p_now->type == RED_H 
			|| p_now->type == RED_I || p_now->type == RED_O)
		{
			if (p_now->next == NULL || p_now->next->type != WORD)
				return ;
			set_red_lst(p_now, node);
		}
		p_now = p_now->next;
	}
}

void	set_red_lst(t_parsing_node *parsing, t_cmd_node *node)
{
	t_type	type;
	void	*red_node;

	type = parsing->type;
	if (type == RED_I || type == RED_H)
	{
		red_node = (t_infile_node*)new_red_node(sizeof(t_infile_node));
		set_infile_node(parsing, (t_infile_node *)red_node);
		if (type == RED_H)
			append_heardoc_node(node->heardoc_node, red_node);
		append_infile_node(node->infile_node, red_node);
		return ;
	}
	red_node = (t_outfile_node*)new_red_node(sizeof(t_outfile_node));
	set_outfile_node(parsing, red_node);
	append_outfile_node(node->outfile_node, red_node);
	
}

void	append_infile_node(t_infile_node *lst, void *red_node)
{
	t_infile_node *now;

	now = lst;
	while (now->next)
		now = now->next;
	now->next = red_node;
}

void	append_heardoc_node(t_infile_node *lst, void *red_node)
{
	t_infile_node *now;

	now = lst;
	while (now->hnext)
		now = now->hnext;
	now->hnext = red_node;
}

void	append_outfile_node(t_outfile_node *lst, void *red_node)
{
	t_outfile_node *now;

	now = (t_outfile_node *)lst;
	while (now->next)
		now = now->next;
	now->next = red_node;

}

void	set_infile_node(t_parsing_node *parsing, void *node)
{
    t_infile_node *file_node = (t_infile_node *)node; 

	if (parsing->type == RED_H)
    {
		file_node->is_heardoc = 1;
        file_node->limiter = parsing->next->str;
    }
    else
    {
	    file_node->file = parsing->next->str;
	    file_node->is_heardoc = 0;
    }
	file_node->hnext = NULL;
	file_node->next = NULL;
}

void	set_outfile_node(t_parsing_node *parsing, void *node)
{
    t_outfile_node *file_node = (t_outfile_node *)node; 

	file_node->type = 0;
	if (parsing->type == RED_A)
		file_node->type = 1;
	file_node->file = parsing->next->str;
	file_node->next = NULL;
}


void	*new_red_node(int size)
{
	void *red_node;

	red_node = calloc(1,size);
	if (!red_node)
		exit (1);
	return (red_node);
}