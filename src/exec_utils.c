/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 19:54:58 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 14:39:36 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static char	*find_exec_in_paths(char **paths, char *cmd_name)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	**s_cmd;
	char	*found_path;
	char	*env_val;

	env_val = ft_getenv("PATH", env);
	if (!env_val)
		return (NULL);
	paths = ft_split(env_val, ':');
	if (!paths)
		return (NULL);
	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd)
	{
		free_ast_argv(paths);
		return (NULL);
	}
	found_path = find_exec_in_paths(paths, s_cmd[0]);
	free_ast_argv(paths);
	free_ast_argv(s_cmd);
	return (found_path);
}

char	*ft_getenv(char *name, char **env)
{
	char	*sub;
	int		i;
	int		j;

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
