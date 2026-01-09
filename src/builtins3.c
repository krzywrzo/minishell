/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 14:21:24 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 17:16:29 by sjesione         ###   ########.fr       */
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

	if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
		old_cwd[0] = '\0';
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
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(key);
		if (val)
			free(val);
		return (1);
	}
	add_or_update_env(&env, key, val);
	return (0);
}

static int	env_list_len(t_env *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

static void	sort_and_print_export(t_env *env)
{
	t_env	**sorted;
	int		len;
	int		i;
	int		j;
	t_env	*tmp;

	len = env_list_len(env);
	if (len == 0)
		return ;
	sorted = malloc(sizeof(t_env *) * len);
	if (!sorted)
		return ;
	i = 0;
	while (env)
	{
		sorted[i++] = env;
		env = env->next;
	}
	i = 0;
	while (i < len)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strncmp(sorted[i]->key, sorted[j]->key,
					ft_strlen(sorted[i]->key) + 1) > 0)
			{
				tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < len)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(sorted[i]->key, 1);
		if (sorted[i]->val)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(sorted[i]->val, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		i++;
	}
	free(sorted);
}

static void	print_export_list(t_env *env)
{
	sort_and_print_export(env);
}

int	export_builtin(char **argv, t_env *env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
	{
		print_export_list(env);
		return (0);
	}
	while (argv[i])
	{
		if (process_export_arg(argv[i], env))
			status = 1;
		i++;
	}
	return (status);
}
