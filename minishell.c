/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 14:46:03 by yejinam           #+#    #+#             */
/*   Updated: 2022/12/15 16:19:40 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
	t_node head;
	char   *line;

	while (1)
	{
		/* 한 줄씩 읽어오기 */
		line = readline("prompt : ");
		// if (line)
		//     printf("%s", line);
		// else
		//     break ; //exit 입력받았을때 종료
		if (line)
		{
			/* 히스토리에 남도록 저장 */
			add_history(line);

			/* str 토큰화 하기 */
			make_token(line, &head);
			
			/* 명령어 실행. 우리가 실행시킬 명령어들이 맞는 지 확인하는 과정 필요 */
			run_command(&head);
			free(line);
		}
	}
	return (0);
}