/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:28:49 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/10 20:15:43 by kwrzosek         ###   ########.fr       */
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
t_ast   *parse_token(t_token *token)
{
    t_strlist   *argv;
    t_ast       *root;
    t_ast       *current;
    t_ast       *cmd;

    argv = NULL;
    root = NULL;
    current = NULL;
    cmd = NULL;

    while (token != NULL)
    {
        if (token->type == TOKEN_WORD || token->type == TOKEN_STRING)
            list_append(&argv, ft_strdup(token->val));
        else if (token->type >= TOKEN_RED_IN && token->type <= TOKEN_HEREDOC)
        {
            t_strlist *cleanup_list = argv;
            
            cmd = build_cmd_from_list(argv);
            if (!cmd)
            {
                free_ast(root);
                free_argv(cleanup_list);
                return (NULL);
            }
            argv = NULL; 
            current = wrap_redir(cmd, token);
            if (!current)
            {
                free_ast(root);
                free_ast(cmd); 
                return (NULL);
            }
            token = token->next;
            if (token != NULL)
                token = token->next;
            else
            {
                free_ast(root);
                free_ast(current);
                return (NULL);
            }
            continue;
        }
        else if (token->type == TOKEN_PIPE)
        {
            if (!current)
            {
                current = build_cmd_from_list(argv);
                if (!current)
                {
                    free_ast(root);
                    return (NULL);
                }
            }
            argv = NULL;
            if (!root)
                root = current;
            else
            {
                t_ast *temp_root = root;
                root = create_pipe_node(root, current);
                if (!root)
                {
                    free_ast(temp_root);
                     return (NULL);
                }
            }
            current = NULL;
        }
        token = token->next;
    }
    if (argv)
    {
        t_strlist *cleanup_list = argv;
        t_ast *last_cmd = build_cmd_from_list(argv);
        
        if (!last_cmd)
        {
            free_ast(root);
            free_argv(cleanup_list);
            return (NULL);
        }
        if (!current)
            current = last_cmd;
        else
        {
            current = merge_ast_nodes(current, last_cmd);
            if (!current)
            {
                free_ast(root);
                free_ast(last_cmd);
                return (NULL);
            }
        }
    }
    if (!root)
    {
        return (current);
    }
    t_ast *original_root = root; 
    root = create_pipe_node(original_root, current);

    if (!root)
    {
        free_ast(original_root);
        free_ast(current);
        return (NULL);
    }
    return (root);
}