/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:38:10 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 18:37:05 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

t_ast	*find_base_command(t_ast *node)
{
	if (node == NULL)
		return (NULL);
	if (node->node_type == NODE_CMD)
		return (node);
	else if (node->node_type == NODE_REDIR || node->node_type == NODE_PIPE)
		return (find_base_command(node->left_node));
	return (NULL);
}

static t_ast	*handle_redir(t_ast *base_node, t_strlist **argv, t_token **token)
{
	t_ast	*cmd;
	t_ast	*node;

	if (*argv)
		cmd = build_cmd_from_list(*argv);
	else if (base_node)
		cmd = base_node;
	else
		cmd = create_cmd_node(NULL);
	if (!cmd)
		return (NULL);
	*argv = NULL;
	node = wrap_redir(cmd, *token);
	if (!node)
	{
		free_ast(cmd);
		return (NULL);
	}
	*token = (*token)->next;
	if (*token)
		*token = (*token)->next;
	return (node);
}

static int	handle_pars_pipe(t_ast **root, t_ast **current, t_strlist **argv)
{
	if (!*current)
	{
		*current = build_cmd_from_list(*argv);
		if (!*current)
		{
			free_argv(*argv);
			return (0);
		}
	}
	*argv = NULL;
	if (!*root)
		*root = *current;
	else
	{
		*root = create_pipe_node(*root, *current);
		if (!*root)
			return (0);
	}
	*current = NULL;
	return (1);
}

static t_ast	*finalize_ast(t_ast *root, t_ast *curr, t_strlist *argv)
{
	t_ast	*last_cmd;

	if (argv)
	{
		last_cmd = build_cmd_from_list(argv);
		if (!last_cmd)
			return (free_all_on_error(root, curr, argv));
		if (!curr)
			curr = last_cmd;
		else
		{
			curr = merge_ast_nodes(curr, last_cmd);
			if (!curr)
				return (free_all_on_error(root, last_cmd, NULL));
		}
	}
	if (!root)
		return (curr);
	root = create_pipe_node(root, curr);
	if (!root)
		return (free_all_on_error(NULL, curr, NULL));
	return (root);
}

t_ast	*parse_token(t_token *token)
{
	t_strlist	*argv;
	t_ast		*root;
	t_ast		*curr;

	argv = NULL;
	root = NULL;
	curr = NULL;
	while (token)
	{
		if (token->type == TOKEN_WORD || token->type == TOKEN_STRING)
			list_append(&argv, ft_strdup(token->val));
		else if (token->type >= TOKEN_RED_IN && token->type <= TOKEN_HEREDOC)
		{
			curr = handle_redir(curr, &argv, &token);
			if (!curr)
				return (free_all_on_error(root, NULL, NULL));
			continue ;
		}
		else if (token->type == TOKEN_PIPE)
			if (!handle_pars_pipe(&root, &curr, &argv))
				return (free_all_on_error(root, curr, NULL));
		token = token->next;
	}
	return (finalize_ast(root, curr, argv));
}
