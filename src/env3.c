/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 17:30:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static void	free_one_env(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	if (node->val)
		free(node->val);
	free(node);
}

void	delete_node(t_env **head, char *key)
{
	t_env	*curr;
	t_env	*prev;

	if (!head || !*head)
		return ;
	curr = *head;
	if (ft_strncmp(curr->key, key, ft_strlen(key) + 1) == 0)
	{
		*head = curr->next;
		free_one_env(curr);
		return ;
	}
	while (curr && ft_strncmp(curr->key, key, ft_strlen(key) + 1) != 0)
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr)
	{
		prev->next = curr->next;
		free_one_env(curr);
	}
}
