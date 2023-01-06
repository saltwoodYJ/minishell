/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 23:38:21 by hyeokim2          #+#    #+#             */
/*   Updated: 2023/01/07 04:16:23 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envp_node	*make_envp_node(char *key, char *value)
{
	t_envp_node	*new;

	new = (t_envp_node *)malloc(sizeof(t_envp_node));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	add_env(t_main_node *main, char *key, char *value)
{
	t_envp_node	*curr;
	t_envp_node	*pre;
	//value가 없으면 NULL로 들어감
	//value 있어야하나 값이 없으면 '\0'로]
	curr = main->ev_lst->next;
	pre = main->ev_lst;
	while (curr)
	{
		//변수에 숫자없는지 확인
		if (!ft_strcmp(curr->key, key)) /* 이미 있는 키값 */
		{
			if (!value)
				return ;
			curr->value = value; //value는 나중에 free해버리니까 복사해야함
			return ;
		}
		curr = curr->next;
		pre = pre->next;
	}
	pre->next = make_envp_node(key, value);
	main->status = 0;
}

void	show_export(t_main_node *main)
{
	t_envp_node	*curr;

	curr = main->ev_lst->next;
	while (curr != NULL)
	{
		if (!curr->value)
			printf("declare -x %s\n", curr->key);
		else
			printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
		curr = curr->next;
	}
	main->status = 0;
}

void	ft_export(t_main_node *main)
{
	char		**cmd;
	char		**args;
	int			i;

	cmd = main->curr->cmd;
	i = 1;
	if (!cmd[i])
		show_export(main);
	while (cmd[i])
	{	
		//'=' 으로 시작할때, key가 _과 문자열로 시작하지 않을 때 에러
		if (search_equal(cmd[i]) == 0 || is_invalid_key(cmd[i], 0)) 
		{
			printf("minishell: export: `%s': not a valid identifier\n", cmd[i]);
			main->status = 1;
		}
		else if (search_equal(cmd[i]) == -1) //=이 없음
		{
			add_env(main, cmd[i], NULL);
			main->status = 0;
		}	
		else
		{
			args = ft_split(cmd[i], '='); //=기준으로 key, value 나누기
			if (args[1] == NULL)
				args[1] = ft_strdup("\0"); 
			add_env(main, args[0], args[1]);
			main->status = 0;
		}
		i++;
	}
	//만약 value가 여러개 오면 뒤에껀 무시
	//만약 =은 있고 value가 없다면 없는 채로 들어감
	//=도 없고 value도 없다면 그냥 export에만 들어감
	// 1. =은 있지만 value가 없는 경우 -> value와 key 모두 저장 key는 "\0"이 들어감
	// 2-1. =도 없고 (이건 맨처음 검사) value도 없는 경우 -> 그냥 value없이 key만 저장
	// 2-2. export 뒤에 = 없이 여러개 들어오면 여러개가 그냥 선언됨.
}