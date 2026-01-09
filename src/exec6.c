/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 14:45:26 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 14:45:57 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

void	run_non_standard(char **argv, char **envp)
{
	char	*path;

	path = argv[0];
	if (access(path, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(127);
	}
	if (access(path, X_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	execve(path, argv, envp);
	perror("execve");
	exit(1);
}

int	is_absolute_relative(char *cmd)
{
	if (!cmd || !cmd[0])
		return (0);
	if (ft_strchr(cmd, '/'))
		return (1);
	return (0);
}
