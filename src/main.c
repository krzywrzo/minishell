/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:26:55 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/25 15:40:48 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	main(void)
{
	char		*input;
	t_token		*head;
	t_ast		*tree;
	
	// FOR TESTING PURPOSE
	int test_count = 0;
	while (test_count < 2)
	{
		input = readline("minishell$ ");
        if (!input)
            break;
        head = read_input(input);
        free(input);
        if (head)
        {
            tree = parse_token(head);
			print_ast(tree, 0);
            if (tree)
                free_ast(tree);
            free_token(head);
        }
		test_count++;
	}

	/*
	while (1)
	{
		input = readline("minishell$ ");
        if (!input)
            break;
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
	return (0);
}
