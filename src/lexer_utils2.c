/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:35:03 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

void	handle_squotes(char *input, t_token *token)
{
	int	i;

	i = 1;
	while ((int)input[i] != 39)
		i++;
	token->type = TOKEN_STRING;
	token->val = ft_substr(input, 0, i + 1);
	token->length = ft_strlen(token->val);
}

void	handle_dquotes(char *input, t_token *token)
{
	int	i;

	i = 1;
	while ((int)input[i] != 34)
		i++;
	token->type = TOKEN_STRING;
	token->val = ft_substr(input, 0, i + 1);
	token->length = ft_strlen(token->val);
}

int	is_separator(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	if (c == '|' || c == '<' || c == '>')
		return (1);
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

void	handle_red_out(char *input, t_token *token)
{
	if (input[1] == '>')
	{
		token->type = TOKEN_APPEND;
		token->val = ft_strdup(">>");
		token->length = 2;
	}
	else
	{
		token->type = TOKEN_RED_OUT;
		token->val = ft_strdup(">");
		token->length = 1;
	}
}

void	handle_red_in(char *input, t_token *token)
{
	if (input[1] == '<')
	{
		token->type = TOKEN_HEREDOC;
		token->val = ft_strdup("<<");
		token->length = 2;
	}
	else
	{
		token->type = TOKEN_RED_IN;
		token->val = ft_strdup("<");
		token->length = 1;
	}
}
