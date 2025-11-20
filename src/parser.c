/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:28:49 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/11/19 19:29:05 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

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

void	list_append(t_strlist **list, char *s)
{
	t_strlist	*tmp;
	t_strlist	*new;

	new = malloc(sizeof(t_strlist));
	if (!new)
		return ;
	new->str = s;
	new->next = NULL;
	while (!(*list))
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
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
    {
        return find_base_command(node->left_node); 
    }
    return NULL; 
}

t_ast   *parse_token(t_token *token)
{
    t_strlist   *argv;
    t_ast       *root;
    t_ast       *current;
    t_ast       *cmd;

    argv = NULL;
    root = NULL;
    current = NULL;

    while (token != NULL)
    {
        if (token->type == TOKEN_WORD)
            list_append(&argv, ft_strdup(token->val));
        else if (token->type >= TOKEN_RED_IN && token->type <= TOKEN_HEREDOC)
        {
            cmd = build_cmd_from_list(argv);
            argv = NULL;
            current = wrap_redir(cmd, token);
            token = token->next;
            if (token != NULL)
                token = token->next;
            else
                return (NULL);
            continue;
        }
        else if (token->type == TOKEN_PIPE)
        {
            if (!current)
            {
                current = build_cmd_from_list(argv);
                if (!current)
                    return (NULL);
            }
            argv = NULL;
            if (!root)
                root = current;
            else
            {
                root = create_pipe_node(root, current);
                if (!root)
                    return (NULL);
            }
            current = NULL;
        }
        token = token->next;
    }
    if (argv)
    {
        t_ast *last_cmd = build_cmd_from_list(argv);
        
        if (!last_cmd)
            return (NULL);
        if (!current)
            current = last_cmd;
        else
        {
            current = merge_ast_nodes(current, last_cmd);
            if (!current)
                return (NULL);
        }
    }
    if (!root)
        return (current);
    root = create_pipe_node(root, current);
    if (!root)
        return (NULL);
    return (root);
}
