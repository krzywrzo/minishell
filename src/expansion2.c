/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int		get_var_name_len(char *s);
char	*get_var_value(char *var_name, t_env *env);
int		handle_exit_status(char *result, int res_len);
int		handle_var(char *str, char *res, int *idx, t_env *env);

static int	is_var_start(char c)
{
	if (ft_isalpha(c) || c == '_')
		return (1);
	return (0);
}

static void	expand_loop(char *str, char *result, int *i, t_env *env)
{
	while (str[i[0]] && i[1] < 4090)
	{
		if (str[i[0]] == '$' && str[i[0] + 1] == '?')
		{
			i[1] += handle_exit_status(result, i[1]);
			i[0] += 2;
		}
		else if (str[i[0]] == '$' && is_var_start(str[i[0] + 1]))
			handle_var(str, result, i, env);
		else
			result[i[1]++] = str[i[0]++];
	}
	result[i[1]] = '\0';
}

char	*expand_variables(char *str, t_env *env)
{
	char	*result;
	int		i[2];

	if (!str)
		return (ft_strdup(""));
	if (!ft_strchr(str, '$'))
		return (ft_strdup(str));
	result = malloc(4096);
	if (!result)
		return (ft_strdup(str));
	i[0] = 0;
	i[1] = 0;
	expand_loop(str, result, i, env);
	return (result);
}
