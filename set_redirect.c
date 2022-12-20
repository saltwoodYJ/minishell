
#include "minishell.h"

int	read_line(char *limiter, int infile, int len)
{
	char	*line;
	char	*line_nl;

	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, limiter, len) == 0)
		{
			free(line);
			return (0);
		}
		line_nl = ft_strjoin(line, "\n", 0, 0);
		write(infile, line_nl, ft_strlen(line_nl));
		free(line_nl);
		free(line);
	}
}

int	make_here_doc(char *limiter)
{
	int		tmp_file;
	char	*tmp_name;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		ft_exit("fork_error", 1);
	tmp_name = ft_strjoin(limiter, "_here_doc.tmp", 0, 0);
	tmp_file = open(tmp_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (pid == 0)
	{
		read_line(limiter, tmp_file, ft_strlen(limiter) + 1);
		exit(0);
	}
	if (pid > 0)
	{
		wait(NULL);
		close(tmp_file);
		tmp_file = open(tmp_name, O_RDONLY);
		dup2(tmp_file, 0);
		close(tmp_file);
		unlink(tmp_name);
		free(tmp_name);
	}
	return (0);
}

void	exec_here_doc(t_node *limiter)
{
	if (limiter->type != WORD)
	{
		printf("unavailable limiter");
		exit(1);
	}
	make_here_doc(limiter->str);
}



void	input_redirect(t_cmd *cmd)
{
	t_node *curr;
	int		input_fd;

	curr = cmd->head->next;
	while (curr != NULL)
	{
		if (curr->type == REDIRECT && (ft_strncmp(curr->str, "<", 2) == 0))
		{
			curr = curr->next;
			input_fd = open(curr->str, O_RDONLY);
			if (input_fd < 0)
			{
				dup2(cmd->info->stdin_fd, 0);
				printf("No such file or directory");
				exit(0);
			}
			else
			{
				dup2(input_fd, 0);
				// close(prev_input);
				// prev_input = input_fd;
			}
		}
		if (curr->type == REDIRECT && (ft_strncmp(curr->str, "<<", 3) == 0))
		{
			curr = curr->next;
			exec_here_doc(curr);
		}
		curr = curr->next;
	}
}

void	output_redirect(t_cmd *cmd)
{
	t_node *curr;
	int		output_fd;

	curr = cmd->head->next;
	while (curr != NULL)
	{
		if (curr->type == REDIRECT && (ft_strncmp(curr->str, ">", 2) == 0 || ft_strncmp(curr->str, ">>", 3) == 0))
		{
			curr = curr->next;
			if (ft_strncmp(curr->str, ">", 2) == 0)
				output_fd = open(curr->str, O_RDWR | O_CREAT | O_TRUNC, 0644);
			else
				output_fd = open(curr->str, O_RDWR | O_CREAT | O_APPEND, 0644);
			if (output_fd < 0)
			{
				dup2(cmd->info->stdout_fd, 1);
				printf("No such file or directory");
				exit(0);
			}
			else
				dup2(output_fd, 1);
		}
		curr = curr->next;
	}
}
