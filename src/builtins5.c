/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static int	export_error(char *arg, char *key, char *val)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	free(key);
	if (val)
		free(val);
	return (1);
}

int	process_export_arg(char *arg, t_env *env)
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
		return (export_error(arg, key, val));
	add_or_update_env(&env, key, val);
	return (0);
}

int	env_list_len(t_env *env)
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

void	sort_env_array(t_env **sorted, int len)
{
	int		i;
	int		j;
	t_env	*tmp;

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
}

void	print_sorted_array(t_env **sorted, int len)
{
	int	i;

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
}
