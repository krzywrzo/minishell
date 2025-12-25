/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:50:55 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/24 01:25:05 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

void	emit_token(t_token **list, char *input, int i)
{
	t_token	*new_token;
	t_token	*last;
	
	if (!list)
		return ;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	pick_handler(input, i, new_token);
	if (*list == NULL)
	{
		*list = new_token;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_token;
}

t_token	*read_input(char *input)
{
	t_token		*head;
	t_token		**list;
	t_token		*last;
	int			i;

	head = NULL;
	list = &head;
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		emit_token(list, input, i);
		last = end_of_list(list);
		i += last->length;
		while (input[i] == ' ')
			i++;
	}
	// print_token(*list);
	return (head);
}

t_token	*end_of_list(t_token **token)
{
	if (!token || !*token)
		return (NULL);
	while ((*token)->next)
		token = &(*token)->next;
	return (*token);
}
