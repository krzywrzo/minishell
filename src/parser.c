/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:28:49 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/25 15:47:18 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static t_ast	*wrap_redir(t_ast *cmd, t_token *token)
{
	t_redir	redir_mode;
	char	*file;

	if (!token->next || token->next->type != TOKEN_WORD)
		return (NULL);
	redir_mode = token_to_mode(token);
	file = ft_strdup(token->next->val);
	return (create_redir_node(cmd, redir_mode, file));
}

void    list_append(t_strlist **list, char *s)
{
    t_strlist   *tmp;
    t_strlist   *new;

    new = malloc(sizeof(t_strlist));
    if (!new)
    {
		if (s)
            free(s); 
        return ;
    }
    new->str = s;
    new->next = NULL;
    if (*list == NULL)
    {
        *list = new;
        return ;
    }
    tmp = *list;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = new;
}
t_strlist *find_list_tail(t_strlist *list)
{
    if (list == NULL)
        return NULL;
    
    while (list->next != NULL)
        list = list->next;
        
    return list;
}

void append_all_strings(t_strlist **dest_list_head, t_strlist *src_list_head)
{
    t_strlist *tail;

    if (src_list_head == NULL)
        return;
    if (*dest_list_head == NULL)
    {
        *dest_list_head = src_list_head;
    }
    else
    {
        tail = find_list_tail(*dest_list_head);
        if (tail != NULL)
            tail->next = src_list_head;
    }
}

t_ast *merge_ast_nodes(t_ast *root_node, t_ast *args_node)
{
    t_ast *base_cmd = find_base_command(root_node); 
    
    if (base_cmd && args_node && args_node->argv)
    {
        append_all_strings(&base_cmd->argv, args_node->argv);
        free(args_node);
    }
    return root_node;
}

t_ast *find_base_command(t_ast *node)
{
    if (node == NULL)
        return NULL;
    if (node->node_type == NODE_CMD)
        return node;
    else if (node->node_type == NODE_REDIR || node->node_type == NODE_PIPE)
        return find_base_command(node->left_node); 
    return NULL; 
}

static t_ast	*handle_redir(t_strlist **argv, t_token **token)
{
	t_ast	*cmd;
	t_ast	*node;

	cmd = build_cmd_from_list(*argv);
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
	else
	{
		free_ast(node);
		return (NULL);
	}
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
			curr = handle_redir(&argv, &token);
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