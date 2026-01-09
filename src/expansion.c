/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:30:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 17:14:07 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static char	*get_var_value(char *var_name, t_env *env)
{
	int	len;

	len = 0;
	while (var_name[len] && (ft_isalnum(var_name[len]) || var_name[len] == '_'))
		len++;
	while (env)
	{
		if (ft_strncmp(env->key, var_name, len) == 0 
			&& env->key[len] == '\0')
			return (env->val);
		env = env->next;
	}
	return (NULL);
}

static int	get_var_name_len(char *s)
{
	int	len;

	len = 0;
	while (s[len] && (ft_isalnum(s[len]) || s[len] == '_'))
		len++;
	return (len);
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	char	*var_val;
	int		i;
	int		var_len;
	int		result_len;

	if (!str || !ft_strchr(str, '$'))
		return (ft_strdup(str));
	result = malloc(4096);
	if (!result)
		return (ft_strdup(str));
	result_len = 0;
	i = 0;
	while (str[i] && result_len < 4090)
	{
		if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			i++;
			var_len = get_var_name_len(&str[i]);
			var_val = get_var_value(&str[i], env);
			if (var_val)
			{
				ft_strlcpy(&result[result_len], var_val, 4096 - result_len);
				result_len += ft_strlen(var_val);
			}
			i += var_len;
		}
		else
		{
			result[result_len++] = str[i++];
		}
	}
	result[result_len] = '\0';
	return (result);
}
