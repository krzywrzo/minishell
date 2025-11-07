/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:28:49 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/11/07 14:32:09 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

void	create_ast(t_token *token)
{
	t_ast	*root;
	while(token->next != NULL)
	{
		if (token->type == 0)
			root = create_cmd_node(token);
		else if (token->type == 1)
			root = create_pipe_node(token);
		else if (token->type == 2 || token->type == 3 || token->type == 4 || token->type == 5)
			root = create_redir_node(token);
		token = (*token).next;
	}
}

t_ast	*create_cmd_node(t_token *token)
{
	t_token	*arg;
	while(token->type == 0 && token->next != NULL)
	{
		arg = (*token).next;

	}
	return (NULL);
}