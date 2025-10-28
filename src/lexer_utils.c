/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:18:54 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/10/23 19:31:05 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

void	pick_handler(char *input, int i, t_token *token)
{
	if (ft_isalpha(input[i]) || ft_isalnum(input[i]))
		handle_general(input + i, token);
	else if (input[i] == '<')
		handle_red_in(input + 1, token);
	else if (input[i] == '>')
		handle_red_out(input + 1, token);
	else if (input[i] == '|')
		handle_pipe(input + 1, token);
	// else
	// {
	// 	token->type = TOKEN_WORD;
	// 	token->val = ft_substr(input, i, 1);
	// 	token->length = 1;
	// }
}


// void	pick_handler(char *input, int i, t_token *token)
// {
// 	if (ft_isalpha(input[i]) == 1)
// 		handle_general(input, token);
// 	else if (input[i] == '<')
// 		handle_red_in(input, token);
// 	else if (input[i] == '>')
// 		handle_red_out(input, token);
// 	else if (input[i] == '|')
// 		handle_pipe(input, token);
// 	// else if (input[i] == '\'')
// 	// 	return (handle_squotes());	// TODO
// 	// else if (input[i] == '"')
// 	// 	return (handle_dquotes());	// TODO	
// 	// else if (input[i] == ' ')
// 	// 	return (1);
// 	// return (0);
// }

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

void	handle_pipe(char *input, t_token *token)
{
	(void)input;
	token->type = TOKEN_PIPE;
	token->val = ft_strdup("|");
	token->length = 1;
}


// void	handle_squotes(t_token *token)
// {


// 	return (0);
// }

// void	handle_dquotes()
// {
// 	return (0);
// }

void	print_token(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		if (token->type == 0)
			printf("#%d [WORD] %s\n", i, token->val);
		else if (token->type == 1)
			printf("#%d [PIPE] %s\n", i, token->val);
		else if (token->type == 2)
			printf("#%d [RED_IN] %s\n", i, token->val);
		else if (token->type == 3)
			printf("#%d [RED_OUT] %s\n", i, token->val);
		else if (token->type == 4)
			printf("#%d [APPEND] %s\n", i, token->val);
		else if (token->type == 5)
			printf("#%d [HEREDOC] %s\n", i, token->val);
		else if (token->type == 6)
			printf("#%d [STRING] %s\n", i, token->val);
		else
			printf("[INVALID TYPE]\n");
		token = token->next;
		i++;
	}
}

