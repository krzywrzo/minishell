/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 19:54:58 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 13:32:05 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**all_path;
	char	**s_cmd;
	char	*path_part;
	char	*exec;
	char	*path_env;

	path_env = ft_getenv("PATH", env);
	if (!path_env)
		return (NULL);
	all_path = ft_split(path_env, ':');
	if (!all_path)
		return (NULL);
	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd)
	{
		free_ast_argv(all_path);
		return (NULL);
	}
	i = -1;
	while (all_path[++i])
	{
		path_part = ft_strjoin(all_path[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_ast_argv(all_path);
			free_ast_argv(s_cmd);
			return (exec);
		}
		free(exec);
	}
	free_ast_argv(all_path);
	free_ast_argv(s_cmd);
	return (NULL);
}

char	*ft_getenv(char *name, char **env)
{
	char *sub;

	int i, j;
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (!sub)
			return (NULL);
		if (ft_strncmp(sub, name, ft_strlen(sub)) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	perror("Command not existing");
	return (NULL);
}