/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 14:46:03 by yejinam           #+#    #+#             */
/*   Updated: 2022/12/27 14:54:23 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*ft_lstnew(int idx, char **cmd)
{
	t_node	*curr;

	curr = (t_node *)malloc(sizeof(t_node));
	if (!curr)
		return (0);
	curr->next = NULL;
	curr->cmd = cmd;
	curr->heardoc_node = NULL;
	curr->infile_node = NULL;
	curr->outfile_node = NULL;
	return (curr);
}

int main(int ac, char **av, char **envp)
{
	t_main_node main_node;
	
	// char   *line;
	if (ac < 0 || av[0] == NULL)
		return (0);
	main_node.ev = envp;
	main_node.input_fd = dup(0);
	main_node.output_fd = dup(1);
	main_node.cmd_num = 2;
	main_node.status = 0;
	main_node.node_head = ft_lstnew(NULL, 0);
	char **arg = {"unset", "HOME"};
	t_node *new = ft_lstnew(0, arg);
	main_node.node_head -> next = new;
	char **arg2 = {"env"};
	t_node *new2 = ft_lstnew(1, arg2);
	new -> next = new2;

	// t_node *new3 = ft_lstnew(">>", REDIRECT);
	// new2 -> next = new3;
	// t_node *new4 = ft_lstnew("b", WORD);
	// new3 -> next = new4;
	// t_node *new5 = ft_lstnew("<", REDIRECT);
	// new4 -> next = new5;
	// t_node *new6 = ft_lstnew("b", WORD);
	// new5 -> next = new6;
	// t_node *new7 = ft_lstnew("|", PIPE);
	// new6 -> next = new7;
	// t_node *new8 = ft_lstnew("ls", WORD);
	// new7 -> next = new8;
	// t_node *new9 = ft_lstnew(">", REDIRECT);
	// new8 -> next = new9;	
	// t_node *new10 = ft_lstnew("c", WORD);
	// new9 -> next = new10;

	// t_node *curr = head->next;
	// while (curr != NULL)
	// {
	// 	printf("%s ", curr->str);
	// 	curr = curr->next;
	// }
	// while (1)
	// {
		/* 한 줄씩 읽어오기 */
		// line = readline("prompt : ");
		// if (line)
		//     printf("%s", line);
		// else
		//     break ; //exit 입력받았을때 종료
		// if (line)
		// {
			/* 히스토리에 남도록 저장 */
			// add_history(line);

			/* str 토큰화 하기 */
			// make_token(line, &head);

			/* 명령어 실행. 우리가 실행시킬 명령어들이 맞는 지 확인하는 과정 필요 */
			run_command(&main_node);
			dup2(main_node.input_fd, 0);
			dup2(main_node.output_fd, 1);
			// free(line);
		// }
	// }
	return (0);
}