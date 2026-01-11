/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 14:21:24 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static void	update_wd_env(t_env *env, char *key, char *new_val)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
		{
			if (env->val)
				free(env->val);
			env->val = ft_strdup(new_val);
			return ;
		}
		env = env->next;
	}
}

static void	update_workdirs(t_env *env, char *old_cwd)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		update_wd_env(env, "OLDPWD", old_cwd);
		update_wd_env(env, "PWD", cwd);
	}
}

static int	cd_to_home(t_env *env)
{
	char	*path;

	path = get_env_val(env, "HOME");
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	return (0);
}

int	cd_builtin(char **argv, t_env *env)
{
	char	old_cwd[1024];

	if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
		old_cwd[0] = '\0';
	if (!argv[1])
	{
		if (cd_to_home(env))
			return (1);
		update_workdirs(env, old_cwd);
		return (0);
	}
	if (chdir(argv[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(argv[1]);
		return (1);
	}
	update_workdirs(env, old_cwd);
	return (0);
}
