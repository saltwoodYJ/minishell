
#include "minishell.h"

int	read_line(char *limiter, int infile)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, limiter, 7) == 0)
		{
			free(line);
			return (0);
		}
		write(infile, line, ft_strlen(line));
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
		read_line(limiter, tmp_file);
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

void	exec_here_doc(t_data *data, t_node *limiter)
{
    
	if (limiter->type != WORD)
	{
		printf("unavailable limiter");
		exit(1);
	}
	make_here_doc(limiter->str);
	exit(0);
}



void	input_redirect(t_data *data)
{
	t_node *temp;
	int		input_fd;

	temp = data->curr;
	while (temp != NULL && temp->type != PIPE)
	{
		if (temp->type == REDIRECT && (ft_strncmp(temp->str, "<", 1) == 0 ))
		{
			temp = temp -> next;
			input_fd = open(temp->str, O_RDONLY);
			if (input_fd < 0)
			{
				dup2(data->stdin_fd, 0);
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
		else if (temp->type == REDIRECT && ft_strncmp(temp->str, "<<", 2) == 0)
		{
			temp = temp -> next;
			exec_here_doc(data, temp);
		}
		temp = temp-> next;
	}
}

void	output_redirect(t_data *data)
{
	t_node *temp;
	int		output_fd;

	temp = data->curr;
	while (temp != NULL && temp->type != PIPE)
	{
		if (temp->type == REDIRECT && (ft_strncmp(temp->str, ">", 1) == 0 || ft_strncmp(temp->str, ">>", 1) == 0))
		{
			temp = temp -> next;
			if (ft_strncmp(temp->str, ">", 1) == 0)
				output_fd = open(temp->str, O_RDWR | O_CREAT | O_TRUNC, 0644);
			else
				output_fd = open(temp->str, O_RDWR | O_CREAT | O_APPEND, 0644);
			if (output_fd < 0)
			{
				dup2(data->stdout_fd, 1);
				printf("No such file or directory");
				exit(0);
			}
			else
				dup2(output_fd, 1);
		}
		temp = temp-> next;
	}
}
