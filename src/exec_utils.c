/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 19:54:58 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 17:17:06 by sjesione         ###   ########.fr       */
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
		if (!tmp)
			return (NULL);
		full_path = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	*found_path;
	char	*env_val;

	env_val = ft_getenv("PATH", env);
	if (!env_val)
		return (NULL);
	paths = ft_split(env_val, ':');
	if (!paths)
		return (NULL);
	found_path = find_exec_in_paths(paths, cmd);
	free_ast_argv(paths);
	return (found_path);
}

char	*ft_getenv(char *name, char **env)
{
	char *sub;
	int i;
	int j;
	int len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (!sub)
			return (NULL);

		if (j == len && ft_strncmp(sub, name, len) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}