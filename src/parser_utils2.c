/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:36:41 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 16:55:52 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

t_strlist	*convert_char_array_to_list(char **array)
{
	t_strlist	*head;
	int			i;

	head = NULL;
	i = 0;
	while (array[i])
	{
		list_append(&head, ft_strdup(array[i]));
		i++;
	}
	return (head);
}

t_ast	*create_cmd_node(t_strlist *list)
{
	t_ast	*node;

	node = calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->node_type = NODE_CMD;
	node->argv = list;
	return (node);
}

char	**list_to_argv(t_strlist *list)
{
	char		**argv;
	t_strlist	*tmp;
	int			i;

	argv = malloc(sizeof(char *) * (get_list_len(list) + 1));
	if (!argv)
		return (NULL);
	i = 0;
	tmp = list;
	while (tmp)
	{
		argv[i++] = tmp->str;
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}
