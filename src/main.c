/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:26:55 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/11/19 19:28:04 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

int	main(void)
{
	char		*input;
	t_token		*head;
	t_ast		*tree;
	while (1)
	{
		input = readline("$minishell ");
		head = read_input(input);			// lexing part
		if (!head)
			return (-1);
		tree = parse_token(head);
		if (!tree)
			return (-1);
		print_ast(tree, 0);
	}

	return (0);
}
