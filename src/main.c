/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 22:26:55 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/10/16 15:32:50 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

int	main(void)
{
	char		*input;

	input = readline("$minishell ");
	if (read_input(input) == -1)
		return (-1);
	// if (return_token(input, list) == 1)
	// 	return(1);
	return (0);
}
