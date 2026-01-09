/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 22:00:53 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 14:34:23 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

t_env	*init_env(char **env)
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;
	int		i;

	head = NULL;
	tail = NULL;
	i = -1;
	while (env[++i])
	{
		new_node = create_node(env, i);
		if (!new_node)
			return (NULL);
		if (head == NULL)
		{
			head = new_node;
			tail = new_node;
		}
		else
		{
			tail->next = new_node;
			tail = new_node;
		}
	}
	return (head);
}

static int	set_node_data(t_env *node, char *str)
{
	char	*eq_pos;

	eq_pos = ft_strchr(str, '=');
	if (eq_pos)
	{
		node->key = ft_substr(str, 0, eq_pos - str);
		node->val = ft_strdup(eq_pos + 1);
	}
	else
	{
		node->key = ft_strdup(str);
		node->val = NULL;
	}
	if (!node->key || (eq_pos && !node->val))
		return (0);
	return (1);
}

static int	fill_node(t_env *node, char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (eq)
	{
		node->key = ft_substr(entry, 0, eq - entry);
		node->val = ft_strdup(eq + 1);
	}
	else
	{
		node->key = ft_strdup(entry);
		node->val = NULL;
	}
	if (!node->key || (eq && !node->val))
		return (0);
	return (1);
}

t_env	*create_node(char **env, int i)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->next = NULL;
	if (!set_node_data(node, env[i]))
	{
		if (node->key)
			free(node->key);
		if (node->val)
			free(node->val);
		free(node);
		return (NULL);
	}
	return (node);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env;
		env = env->next;
		free(tmp);
	}
}
