/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:04:33 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/11/16 17:54:53 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

void	print_token(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		if (token->type == 0)
			printf("#%d [WORD] %s\n", i, token->val);
		else if (token->type == 1)
			printf("#%d [PIPE] %s\n", i, token->val);
		else if (token->type == 2)
			printf("#%d [RED_IN] %s\n", i, token->val);
		else if (token->type == 3)
			printf("#%d [RED_OUT] %s\n", i, token->val);
		else if (token->type == 4)
			printf("#%d [APPEND] %s\n", i, token->val);
		else if (token->type == 5)
			printf("#%d [HEREDOC] %s\n", i, token->val);
		else if (token->type == 6)
			printf("#%d [STRING] %s\n", i, token->val);
		else
			printf("[INVALID TYPE]\n");
		token = token->next;
		i++;
	}
}

void	print_ast(t_ast *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i++ < depth)
		printf("  ");
	if (node->node_type == NODE_CMD)
	{
		printf("CMD: ");
		for (int j = 0; node->argv && node->argv[j]; j++)
			printf("%c", node->argv[j]);
		printf("\n");
	}
	else if (node->node_type == NODE_PIPE)
		printf("PIPE\n");
	else if (node->node_type == NODE_REDIR)
		printf("REDIR %s\n", node->file);
	print_ast(node->left_node, depth + 1);
	print_ast(node->right_node, depth + 1);
}

void	print_argv(char *argv)
{
	int i = 0;
	while (argv[i])
	{
		printf("%c", argv[i]);
		i++;
	}
	printf("\n");
}