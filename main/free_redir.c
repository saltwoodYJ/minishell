/* ************************************************************************** */
/*															 */
/*											 :::	 ::::::::   */
/*   free_redir.c								:+:	 :+:	:+:   */
/*										  +:+ +:+		+:+	*/
/*   By: yejinam <marvin@42.fr>				 +#+  +:+	  +#+	   */
/*									   +#+#+#+#+#+   +#+		 */
/*   Created: 2023/01/14 16:16:31 by yejinam		 #+#	#+#		   */
/*   Updated: 2023/01/14 16:16:32 by yejinam		###   ########.fr	  */
/*															 */
/* ************************************************************************** */

#include "minishell.h"

void	free_one_input(t_infile_node *input)
{
	if (input == 0)
		return ;
	if (input->file)
		free(input->file);
	if (input->limiter)
		free(input->limiter);
	free(input);
}

void	ft_input_clear(t_infile_node **inputs)
{
	t_infile_node	*curr;

	if (*inputs == 0)
		return ;
	while (*inputs != 0)
	{
		curr = (*inputs)->next;
		free_one_input(*inputs);
		*inputs = curr;
	}
	*inputs = 0;
}

void	free_one_output(t_outfile_node *output)
{
	if (output == 0)
		return ;
	if (output->file)
		free(output->file);
	free(output);
}

void	ft_output_clear(t_outfile_node **outputs)
{
	t_outfile_node	*curr;

	if (*outputs == 0)
		return ;
	while (*outputs)
	{
		curr = (*outputs)->next;
		free_one_output(*outputs);
		*outputs = curr;
	}
	*outputs = 0;
}
