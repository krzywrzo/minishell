/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:18:54 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static int	skip_cmd_subst(char *input, int len)
{
	int	paren_depth;

	len += 2;
	paren_depth = 1;
	while (input[len] && paren_depth > 0)
	{
		if (input[len] == '(')
			paren_depth++;
		else if (input[len] == ')')
			paren_depth--;
		len++;
	}
	return (len);
}

static int	get_word_len(char *input, int i)
{
	int	len;

	len = 0;
	while (input[i + len] && !is_separator(input[i + len]))
	{
		if (input[i + len] == '$' && input[i + len + 1] == '(')
			len = skip_cmd_subst(input + i, len) - i;
		else
			len++;
	}
	return (len);
}

static void	handle_word(char *input, int i, t_token *token)
{
	int	len;

	len = get_word_len(input, i);
	token->type = TOKEN_WORD;
	token->val = ft_substr(input, i, len);
	token->length = len;
}

void	handle_general(char *input, t_token *token)
{
	int	i;
	int	j;

	i = 0;
	if (input[i] && input[i] != ' ')
	{
		j = 0;
		while (input[j] && input[j] != ' ')
			j++;
		token->type = TOKEN_WORD;
		token->val = ft_substr(input, i, j - i);
		token->length = ft_strlen(token->val);
	}
	else
		token->length = 0;
}

void	pick_handler(char *input, int i, t_token *token)
{
	if (input[i] == ' ' || input[i] == '\t')
		handle_general(input + i, token);
	else if (input[i] == '<')
		handle_red_in(input + i, token);
	else if (input[i] == '>')
		handle_red_out(input + i, token);
	else if (input[i] == '|')
		handle_pipe(token);
	else if (input[i] == '\'')
		handle_squotes(input + i, token);
	else if (input[i] == '"')
		handle_dquotes(input + i, token);
	else
		handle_word(input, i, token);
}
