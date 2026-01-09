/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 14:21:24 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 14:23:18 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static void	update_workdirs(t_env *env, char *old_cwd)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		update_wd_env(env, "OLDPWD", old_cwd);
		update_wd_env(env, "PWD", cwd);
	}
}

int	cd_builtin(char **argv, t_env *env)
{
	char	*path;
	char	old_cwd[1024];

	getcwd(old_cwd, sizeof(old_cwd));
	if (!argv[1])
	{
		path = get_env_val(env, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		path = argv[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	update_workdirs(env, old_cwd);
	return (0);
}

static int	process_export_arg(char *arg, t_env *env)
{
	char	*key;
	char	*val;
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		val = ft_strdup(eq + 1);
	}
	else
	{
		key = ft_strdup(arg);
		val = NULL;
	}
	if (!is_valid_key(key))
	{
		ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
		free(key);
		if (val)
			free(val);
		return (1);
	}
	add_or_update_env(&env, key, val);
	return (0);
}

int	export_builtin(char **argv, t_env *env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
		return (env_builtin(env));
	while (argv[i])
	{
		if (process_export_arg(argv[i], env))
			status = 1;
		i++;
	}
	return (status);
}
