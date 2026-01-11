/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:31:07 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static t_env	*new_env_node(char *key, char *val)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (val)
		node->val = ft_strdup(val);
	else
		node->val = NULL;
	node->next = NULL;
	return (node);
}

static void	update_existing(t_env *node, char *val)
{
	if (val)
	{
		if (node->val)
			free(node->val);
		node->val = ft_strdup(val);
	}
}

static void	free_key_val(char *key, char *val)
{
	free(key);
	if (val)
		free(val);
}

static t_env	*find_or_last(t_env *head, char *key, int *found)
{
	t_env	*curr;

	curr = head;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(key) + 1) == 0)
		{
			*found = 1;
			return (curr);
		}
		if (!curr->next)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

void	add_or_update_env(t_env **head, char *key, char *val)
{
	t_env	*curr;
	int		found;

	found = 0;
	if (!*head)
	{
		*head = new_env_node(key, val);
		return (free_key_val(key, val));
	}
	curr = find_or_last(*head, key, &found);
	if (found)
		update_existing(curr, val);
	else
		curr->next = new_env_node(key, val);
	free_key_val(key, val);
}
