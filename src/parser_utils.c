/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:25:44 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 14:48:11 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

t_redir	token_to_mode(t_token *token)
{
	if (token->type == TOKEN_APPEND)
		return (REDIR_APPEND);
	else if (token->type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	else if (token->type == TOKEN_RED_IN)
		return (REDIR_IN);
	else if (token->type == TOKEN_RED_OUT)
		return (REDIR_OUT);
	return (0);
}

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->node_type = NODE_PIPE;
	node->argv = NULL;
	node->file = NULL;
	node->redir_type = -1;
	node->left_node = left;
	node->right_node = right;
	return (node);
}

t_ast	*create_redir_node(t_ast *cmd, t_redir mode, char *file)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->node_type = NODE_REDIR;
	node->argv = NULL;
	node->file = file;
	node->redir_type = mode;
	node->left_node = cmd;
	node->right_node = NULL;
	return (node);
}

t_ast	*build_cmd_from_list(t_strlist *list)
{
	t_ast	*node;

	if (!list)
		return (NULL);
	node = create_cmd_node(list);
	if (!node)
		return (NULL);
	return (node);
}

static int	get_list_len(t_strlist *list)
{
	int	len;

	len = 0;
	while (list)
	{
		len++;
		list = list->next;
	}
	return (len);
}
