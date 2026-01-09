/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:30:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 13:41:21 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

t_ast	*free_all_on_error(t_ast *root, t_ast *current, t_strlist *argv)
{
	if (root)
		free_ast(root);
	if (current)
		free_ast(current);
	if (argv)
		free_argv(argv);
	return (NULL);
}
