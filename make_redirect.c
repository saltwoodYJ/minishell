
#include "minishell.h"

int	read_line(char *limiter, int infile)
{
	char	*line;
	char	*line_nl;

	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			return (0);
		}
		line_nl = ft_strjoin(line, "\n");
		write(infile, line_nl, ft_strlen(line_nl));
		free(line_nl);
		free(line);
	}
}

char	*make_heredoc(char *limiter)
{
	int		tmp_file;
	char	*tmp_name;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		printf("fork_error");
	tmp_name = ft_strjoin3(".", limiter, "_heredoc.tmp");
	tmp_file = open(tmp_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (pid == 0)
	{
		read_line(limiter, tmp_file);
		exit(0);
	}
	if (pid > 0)
	{
		wait(NULL);
		close(tmp_file);
	}
	return (tmp_name);
}

void    set_heredoc(t_main_node *main)
{
    t_infile_node *curr;
    char          *file_name;

    curr = main->curr->heardoc_node->hnext;
    while (curr != NULL)
    {
        file_name = make_heredoc(curr->limiter);
        curr->file = file_name;
        curr = curr->hnext;
    }
}

void	input_redirect(t_main_node *main)
{
	t_infile_node	*curr;
	int				input_fd;

	curr = main->curr->infile_node;
	while (curr != NULL)
	{
		input_fd = open(curr->file, O_RDONLY);
		if (input_fd >= 0)
        {
			dup2(input_fd, 0);
            close(input_fd);
            if (curr->is_heardoc == 1)
                unlink(curr->file);
        }
	    curr = curr->next;
	}
}

void	output_redirect(t_main_node *main)
{
	t_outfile_node	*curr;
	int		output_fd;

	curr = main->curr->outfile_node;
	while (curr != NULL)
	{
		if (curr->type == 0)
			output_fd = open(curr->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		else
			output_fd = open(curr->file, O_RDWR | O_CREAT | O_APPEND, 0644);
        if (output_fd >= 0)
        {
		    dup2(output_fd, 1);
            close(output_fd);
        }
		curr = curr->next;
	}
}
