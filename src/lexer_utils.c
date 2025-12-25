/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:18:54 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/24 01:25:15 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

void	pick_handler(char *input, int i, t_token *token)
{
	if (ft_issign(input[i]))
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
	{
		token->type = TOKEN_WORD;
		token->val = ft_substr(input, i, 1);
		token->length = 1;
	}
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

void	handle_pipe(t_token *token)
{
	token->type = TOKEN_PIPE;
	token->val = ft_strdup("|");
	token->length = 1;
}

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

int	ft_issign(char str)
{
	if(ft_isalnum(str) || str == '-')
		return 1;
	else 
		return 0;
}