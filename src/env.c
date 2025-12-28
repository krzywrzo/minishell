/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 22:00:53 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/28 14:18:43 by kwrzosek         ###   ########.fr       */
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
    i = 0;
    while (env[i])
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
        i++;
    }
    return (head);
}


t_env	*create_node(char **env, int i)
{
	t_env	*node;
	int		j;
	char	*curr_env;

	node = malloc(sizeof(t_env));
	curr_env = malloc(sizeof(char) * (ft_strlen(env[i]) + 1));
	if (!node || !curr_env)
		return (NULL);
	j = 0;
	ft_strlcpy(curr_env, env[i], ft_strlen(env[i]) + 1);
	while (curr_env[j] && curr_env[j] != '=')
		j++;
	node->key = malloc(sizeof(char) * (j + 1));
	if (!node->key)
	{
		free(node);
		free(curr_env);
		return(NULL);
	}
	ft_strlcpy(node->key, curr_env, j + 1);
	node->val = ft_getenv(node->key, env);
	free(curr_env);
	return(node);
}