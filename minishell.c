/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yejinam <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 14:46:03 by yejinam           #+#    #+#             */
/*   Updated: 2022/12/15 14:46:06 by yejinam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



int main(int ac, char **av, char **envp)
{
    char *str;
    while(1)
    {
        str = readline("prompt : ");
        if (str)
            printf("%s", str);
        else
            break ; //exit 입력받았을때 종료
        add_history(str);
        //실행
        free(str);
    }
    return(0);
}