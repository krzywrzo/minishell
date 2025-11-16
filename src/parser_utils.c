/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:25:44 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/11/16 18:01:55 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

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

void	free_strlist(t_strlist *list)
{
	t_strlist	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list);
		list = tmp;
	}
}

t_ast	*build_cmd_from_list(t_strlist *list)
{
	char	**argv;
	t_ast	*node;

	if (!list)
		return (NULL);
	argv = list_to_argv(list);
	if (!argv)
		return (NULL);
	node = create_cmd_node(argv);
	free_strlist(list);
	return (node);
}


char	**list_to_argv(t_strlist *list)
{
	t_strlist	*tmp;
	char		**argv;
	int			count;
	int			i;

	count = 0;
	tmp = list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	tmp = list;
	i = 0;
	while (tmp)
	{
		argv[i++] = ft_strdup(tmp->str);
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}


t_ast	*create_cmd_node(char **argv)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->node_type = NODE_CMD;
	node->argv = *argv;
	node->file = NULL;
	node->redir_type = -1;
	node->left_node = NULL;
	node->right_node = NULL;
	return (node);
}

