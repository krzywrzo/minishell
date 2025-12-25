/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:25:44 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/25 15:41:50 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

t_redir	token_to_mode(t_token *token)
{
	if (token->type == TOKEN_APPEND)
		return(REDIR_APPEND);
	else if (token->type == TOKEN_HEREDOC)
		return(REDIR_HEREDOC);
	else if (token->type == TOKEN_RED_IN)
		return(REDIR_IN);
	else if (token->type == TOKEN_RED_OUT)
		return(REDIR_OUT);
	return 0;
}

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return NULL;
	node->node_type = NODE_PIPE;
	node->argv = NULL;
	node->file = NULL;
	node->redir_type = -1;
	node->left_node = left;
	node->right_node = right;
	return(node);
}
t_ast	*create_redir_node(t_ast *cmd, t_redir mode, char *file)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return NULL;
	node->node_type = NODE_REDIR;
	node->argv = NULL;
	node->file = file;
	node->redir_type = mode;
	node->left_node = cmd;
	node->right_node = NULL;
	return(node);
}

t_ast   *build_cmd_from_list(t_strlist *list)
{
    t_ast   *node;

    if (!list)
        return (NULL);
    node = create_cmd_node(list); 
    if (!node)
        return (NULL);
    return (node);
}

char    **list_to_argv(t_strlist *list)
{
    t_strlist   *curr;
    t_strlist   *next_node; 
    char        **argv;
    int         count;
    int         i;

    count = 0;
    curr = list;
    while (curr) {
        count++;
        curr = curr->next;
    }
    argv = malloc(sizeof(char *) * (count + 1));
    if (!argv) {
		// free_strlist(curr);
        free_argv(list);
        return (NULL);
    }
    curr = list;
    i = 0;
    while (curr)
    {
        argv[i++] = curr->str;
        next_node = curr->next;
        free(curr);
        curr = next_node;
    }
	// free_strlist(curr);
    argv[i] = NULL;
    return (argv);
}
t_strlist *convert_char_array_to_list(char **array)
{
    t_strlist *head = NULL;
    int i;
	
	i = 0;
    while (array[i])
    {
        list_append(&head, ft_strdup(array[i]));
        i++;
    }
    return (head);
}

t_ast *create_cmd_node(t_strlist *list)
{
    t_ast *node;
    
    node = calloc(1, sizeof(t_ast));
    if (!node)
        return (NULL);
        
    node->node_type = NODE_CMD;
    node->argv = list; // ✅ Correct: Assigning t_strlist* to t_strlist*
    // node->argv is now the owner of the list memory.
    
    return (node);
}