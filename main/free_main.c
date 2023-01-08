/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 05:18:52 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/07 05:54:18 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_main(t_main_node *main)
{
    ft_node_clear(&(main->node_head));
	free(main->node_head);
	ft_input_clear(&(main->heardoc_node));
	free(main->heardoc_node);
}
