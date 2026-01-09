/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:35:03 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 14:58:30 by sjesione         ###   ########.fr       */
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
