#include "minishell.h"

void	make_cmd_list(t_parsing_node *parse, t_main_node *main)
{
	t_parsing_node  *p_now;
	t_cmd_node	  	*c_now;
	int			 i;

	p_now = parse;
	i = 0;

	main->node_head = malloc(sizeof(t_cmd_node));
	c_now = main->node_head;
	while (p_now)
	{
		if (p_now->type == PIPE)
		{
			i++;
			c_now->next = make_cmd_node(&p_now, &c_now, i);
			c_now = c_now->next;
		}
		p_now = p_now->next;
	}
	main->cmd_num = i;	
}

t_cmd_node	*make_cmd_node(t_parsing_node **p_now, t_cmd_node **c_now, int i)
{
	t_cmd_node  *node;
	node = malloc(sizeof(t_cmd_node));
	if (!node)
		exit (1);
	init_cmd_node(node);
	node->idx = i;
	node->cmd = make_cmd(*p_now);
	return (node);
}
char	**make_cmd(t_parsing_node *parsing)
{
	t_parsing_node  *now;
	char			**cmd;
	int			 index;
	int			 cmd_num;
		
	now = parsing->next;
	index = 0;
	cmd_num = get_cmd_num(parsing);
	cmd = malloc(sizeof(char *) * (cmd_num + 1));
	if (!cmd)
		exit (1);
	while (now && now->type != PIPE)
	{
		if (now->type == WORD)
		{
			cmd[index] = now->str;
			index++;
		}
		now=now->next;
	}
	cmd[index] = NULL;
	return (cmd);
}

int get_cmd_num(t_parsing_node *parsing)
{
	t_parsing_node  *now;
	int			 i;

	i = 0;
	now = parsing->next;
	while (now && now->type != PIPE)
	{
		if (now->type == WORD)
			i++;
		now=now->next;
	}
	return (i);
}

void	init_cmd_node(t_cmd_node *node)
{
	node->idx = 0;
	node->cmd = NULL;
	node->heardoc_node = NULL;
	node->infile_node = NULL;
	node->outfile_node = NULL;
	node->next = NULL;
}