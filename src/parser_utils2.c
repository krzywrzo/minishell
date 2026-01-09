/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:36:41 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 13:37:39 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	node->argv = list; // ✅ Correct: Assigning t_strlist* to t_strlist*
	// node->argv is now the owner of the list memory.
	return (node);
}
