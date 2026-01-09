/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:28:37 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 13:41:12 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	unset_builtin(char **argv, t_env *env)
{
	int	i;
	int	list_len;

	list_len = list_size(env);
	i = 1;
	while (argv[i])
	{
		if (i < list_len)
			return (1);
		delete_node(&env, argv[i]);
		i++;
	}
	return (0);
}

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	exit_builtin(char **argv, t_env *env)
{
	int	code;

	ft_putstr_fd("exit\n", 1);
	if (!argv[1])
	{
		free_env(env);
		exit(0);
	}
	if (is_numeric(argv[1]))
	{
		if (argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		code = ft_atoi(argv[1]);
		free_env(env);
		exit(code % 256);
	}
	ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
	free_env(env);
	exit(255);
}

static int	is_valid_key(char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*get_env_val(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env->val);
		env = env->next;
	}
	return (NULL);
}
