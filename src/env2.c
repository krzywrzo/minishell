/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:31:07 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 14:34:32 by sjesione         ###   ########.fr       */
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

void	add_or_update_env(t_env **head, char *key, char *val)
{
	t_env	*curr;

	if (!*head)
	{
		*head = new_env_node(key, val);
		return ;
	}
	curr = *head;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(key) + 1) == 0)
		{
			update_existing(curr, val);
			free(key);
			if (val)
				free(val);
			return ;
		}
		if (!curr->next)
			break ;
		curr = curr->next;
	}
	curr->next = new_env_node(key, val);
}

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
