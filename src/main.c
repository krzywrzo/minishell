/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:26:55 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/29 15:30:12 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

void	print_env(t_env *envl)
{
	if (!envl)
		printf("List is empty\n");
	while(envl)
	{
		printf("KEY: %s VAL: %s\n", envl->key, envl->val);
		envl = envl->next;
	}
}

int	main(int argc, char **argv, char **env)
{
	char		*input;
	t_token		*head;
	t_ast		*tree;		// pointer for the root of the tree
	t_env 		*envl;		// linked list of env vars

	(void)argc;
	(void)argv;
	envl = init_env(env);
	// print_env(envl);
	// FOR TESTING PURPOSE
	int test_count = 0;
	while (test_count < 2)
	{
		input = readline("minishell$ ");
        if (!input)
            break;
		if (ft_strlen(input) > 0)
			add_history(input);
        head = read_input(input);
        free(input);
        if (head)
        {
            tree = parse_token(head);
			// print_ast(tree, 0);
            if (!tree)
                free_ast(tree);
            free_token(head);
			// order_66(tree);
			if (order_66(tree, envl) != 0)		// execution part
			{
				free_ast(tree);
				return(-1);
			}	
        }
		test_count++;
	}

	/*
	while (1)
	{
		input = readline("minishell$ ");
        if (!input)
            break;
		if (ft_strlen(input) > 0)
		add_history(input);
        head = read_input(input);
        free(input);
        if (head)
        {
            tree = parse_token(head);
            if (tree)
                free_ast(tree);
            free_token(head);
        }
	}
	*/
	// free_env(envl);		// TODO: free_env()
	return (0);
}
