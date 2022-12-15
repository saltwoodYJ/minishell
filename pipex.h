/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeokim2 <hyeokim2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 19:48:30 by hyeokim2          #+#    #+#             */
/*   Updated: 2022/12/15 17:27:49 by hyeokim2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

size_t	ft_strlen(char *str);
char	*ft_strjoin(char const *s1, char const *s2, int i, int j);
char	**ft_split(char const *s, char c);
size_t	ft_double_strlen(char **str);
void	*ft_memset(void *b, int c, size_t len);
char	*ft_search_path(char **envp, char *first_cmd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_exit(char *str, int code);
int		make_fork(char *av, char **envp);
void	ft_file_open(char *filename1, char *filename2, int *file1, int *file2);
void	ft_putstr_err(char *s1, char *s2);
void	exec_here_doc(int ac, char **av, char **envp);
void	ft_free(char **s1, char **s2);

#endif 