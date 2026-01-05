/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 22:00:53 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/05 15:09:44 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

t_env   *init_env(char **env)
{
    t_env   *head;
    t_env   *tail;
    t_env   *new_node;
    int     i;

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

t_env   *create_node(char **env, int i)
{
    t_env   *node;
    int     j;
    char    *curr_env;

    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    curr_env = ft_strdup(env[i]);
    if (!curr_env)
    {
        free(node);
        return (NULL);
    }
    j = 0;
    while (curr_env[j] && curr_env[j] != '=')
        j++;
    node->key = ft_substr(curr_env, 0, j);
    if (!node->key)
    {
        free(curr_env);
        free(node);
        return (NULL);
    }
    if (curr_env[j] == '=')
        node->val = ft_strdup(&curr_env[j + 1]); 
    else
        node->val = NULL;
    node->next = NULL;
    free(curr_env);
    return (node);
}
void	free_env(t_env *env)
{
	t_env *tmp;

	while (env != NULL)
	{
		tmp = env;
		env = env->next;
		free(tmp);
	}
}

/*
** Removes a node from the environment list by key.
** Usage: delete_node(&env, "VAR_NAME");
*/
void delete_node(t_env **head, char *key)
{
    t_env   *current;
    t_env   *prev;

    if (!head || !*head)
        return ;
    current = *head;
    if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
    {
        *head = current->next;
        free(current->key);
        if (current->val)
            free(current->val);
        free(current);
        return ;
    }
    while (current && ft_strncmp(current->key, key, ft_strlen(key) + 1) != 0)
    {
        prev = current;
        current = current->next;
    }
    if (!current)
        return ;
    prev->next = current->next;
    free(current->key);
    if (current->val)
        free(current->val);
    free(current);
}

static t_env *new_env_node(char *key, char *val)
{
    t_env *node;

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

void add_or_update_env(t_env **head, char *key, char *val)
{
    t_env *current;
    t_env *new_node;

    if (!*head)
    {
        *head = new_env_node(key, val);
        return ;
    }
    current = *head;
    while (current)
    {
        if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
        {
            if (val) 
            {
                if (current->val)
                    free(current->val);
                current->val = ft_strdup(val);
            }
            return ;
        }
        if (current->next == NULL)
            break ;
        current = current->next;
    }
    new_node = new_env_node(key, val);
    if (new_node)
        current->next = new_node;
}
