/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:30:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	get_var_name_len(char *s)
{
	int	len;

	len = 0;
	while (s[len] && (ft_isalnum(s[len]) || s[len] == '_'))
		len++;
	return (len);
}

char	*get_var_value(char *var_name, t_env *env)
{
	int	len;

	len = get_var_name_len(var_name);
	while (env)
	{
		if (ft_strncmp(env->key, var_name, len) == 0 && env->key[len] == '\0')
			return (env->val);
		env = env->next;
	}
	return (NULL);
}

int	handle_exit_status(char *result, int res_len)
{
	char	*exit_code;
	int		len;

	exit_code = ft_itoa(g_exit_status);
	if (exit_code)
	{
		ft_strlcpy(&result[res_len], exit_code, 4096 - res_len);
		len = ft_strlen(exit_code);
		free(exit_code);
		return (len);
	}
	return (0);
}

int	handle_var(char *str, char *res, int *idx, t_env *env)
{
	int		var_len;
	char	*var_val;

	idx[0]++;
	var_len = get_var_name_len(&str[idx[0]]);
	var_val = get_var_value(&str[idx[0]], env);
	if (var_val)
	{
		ft_strlcpy(&res[idx[1]], var_val, 4096 - idx[1]);
		idx[1] += ft_strlen(var_val);
	}
	idx[0] += var_len;
	return (0);
}

static int	is_var_start(char c)
{
	if (ft_isalpha(c) || c == '_')
		return (1);
	return (0);
}
