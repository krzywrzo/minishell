/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:32:39 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 13:42:07 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	is_state_changing(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (1);
	return (0);
}

int	handle_error(void) //	TODO: handle execve error
{
	return (-1);
}

char	*join_env_str(char *key, char *val)
{
	char	*tmp;
	char	*res;

	if (!key || !val)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, val);
	free(tmp);
	return (res);
}

char	**convert_list_to_arr(t_env *env)
{
	char	**env_arr;
	int		env_size;
	int		i;

	env_size = list_size(env);
	env_arr = ft_calloc(env_size + 1, sizeof(char *));
	if (!env_arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->val)
		{
			env_arr[i] = join_env_str(env->key, env->val);
			if (env_arr[i] == NULL)
			{
				free_ast_argv(env_arr);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	return (env_arr);
}

int	list_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}
