/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:26:12 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/25 15:32:08 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

void    free_token(t_token *token)
{
    t_token *next_node;

    while (token != NULL)
    {
        next_node = token->next;
        if (token->val != NULL)
        {
            free(token->val);
            token->val = NULL;
        }
        free(token);
        token = next_node; 
	}
}

void free_ast_argv(char **argv)
{
	int	i;

    if (argv == NULL)
        return;
	while (argv[i] != NULL)
	{
		free(argv[i]);
		i++;
	}
    free(argv);
}

void	free_argv(t_strlist *argv)
{
	if (argv != NULL)
	{
		t_strlist *tmp;

   		while (argv != NULL)
    	{
       		tmp = argv;
			free(argv->str);
       		argv = argv->next;
       		free(tmp);
    	}
	}
}

void	free_ast(t_ast *root)
{
	if (root == NULL)
		return ;
	free_ast(root->left_node);
	free_ast(root->right_node);
	if (root->argv != NULL)
		free_argv(root->argv);
	if (root->val != NULL)
		free(root->val);
	if (root->file != NULL)
		free(root->file);
	free(root);
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

 t_ast *free_all_on_error(t_ast *root, t_ast *current, t_strlist *argv)
{
    if (root)
        free_ast(root);
    if (current)
        free_ast(current);
    if (argv)
        free_argv(argv);
    return (NULL);
}
