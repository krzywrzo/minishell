/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 15:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static int	is_fully_quoted(char *str, char quote)
{
	if (!str || str[0] != quote)
		return (0);
	if (ft_strlen(str) < 2)
		return (0);
	if (str[ft_strlen(str) - 1] != quote)
		return (0);
	return (1);
}

static char	*process_quoted_seg(char *str, int *i, t_env *env)
{
	int		seg_start;
	char	*segment;
	char	*expanded;
	char	quote;

	quote = str[*i];
	seg_start = *i + 1;
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	segment = ft_substr(str, seg_start, *i - seg_start);
	if (quote == '"')
	{
		expanded = expand_variables(segment, env);
		free(segment);
	}
	else
		expanded = segment;
	if (str[*i] == quote)
		(*i)++;
	return (expanded);
}

static char	*process_unquoted_seg(char *str, int *i, t_env *env)
{
	int		seg_start;
	char	*segment;
	char	*expanded;

	seg_start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	segment = ft_substr(str, seg_start, *i - seg_start);
	expanded = expand_variables(segment, env);
	free(segment);
	return (expanded);
}

static char	*process_segment(char *str, int *i, t_env *env)
{
	if (str[*i] == '\'' || str[*i] == '"')
		return (process_quoted_seg(str, i, env));
	return (process_unquoted_seg(str, i, env));
}

char	*process_string_with_quotes(char *str, t_env *env)
{
	char	*result;
	char	*segment;
	int		i;
	int		result_len;

	if (!str)
		return (ft_strdup(""));
	result = malloc(4096);
	if (!result)
		return (ft_strdup(str));
	result_len = 0;
	i = 0;
	while (str[i] && result_len < 4090)
	{
		segment = process_segment(str, &i, env);
		if (segment)
		{
			ft_strlcpy(&result[result_len], segment, 4096 - result_len);
			result_len += ft_strlen(segment);
			free(segment);
		}
	}
	result[result_len] = '\0';
	return (result);
}
