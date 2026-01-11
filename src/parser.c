/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:17:43 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

t_ast	*wrap_redir(t_ast *cmd, t_token *token)
{
	t_redir	redir_mode;
	char	*file;
	char	*processed_file;

	if (!token->next || (token->next->type != TOKEN_WORD
			&& token->next->type != TOKEN_STRING))
		return (NULL);
	redir_mode = token_to_mode(token);
	file = token->next->val;
	if (token->next->type == TOKEN_STRING)
	{
		if (file[0] == '\'' && file[ft_strlen(file) - 1] == '\'')
			processed_file = ft_substr(file, 1, ft_strlen(file) - 2);
		else if (file[0] == '"' && file[ft_strlen(file) - 1] == '"')
			processed_file = ft_substr(file, 1, ft_strlen(file) - 2);
		else
			processed_file = ft_strdup(file);
	}
	else
		processed_file = ft_strdup(file);
	return (create_redir_node(cmd, redir_mode, processed_file));
}

void	list_append(t_strlist **list, char *s)
{
	t_strlist	*tmp;
	t_strlist	*new;

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

t_strlist	*find_list_tail(t_strlist *list)
{
	if (list == NULL)
		return (NULL);
	while (list->next != NULL)
		list = list->next;
	return (list);
}

void	append_all_strings(t_strlist **dest_list_head, t_strlist *src_list_head)
{
	t_strlist	*tail;

	if (src_list_head == NULL)
		return ;
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

t_ast	*merge_ast_nodes(t_ast *root_node, t_ast *args_node)
{
	t_ast	*base_cmd;

	base_cmd = find_base_command(root_node);
	if (base_cmd && args_node && args_node->argv)
	{
		append_all_strings(&base_cmd->argv, args_node->argv);
		free(args_node);
	}
	return (root_node);
}
