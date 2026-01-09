/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:04:33 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 13:40:12 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

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
	t_strlist	*current_arg;
	int			i;
	char		*redir;

	if (!node)
		return ;
	current_arg = node->argv;
	i = 0;
	while (i++ < depth)
		printf("  ");
	if (node->node_type == NODE_CMD)
	{
		printf("CMD: ");
		while (current_arg != NULL)
		{
			printf(" \"%s\"", current_arg->str);
			current_arg = current_arg->next;
		}
		printf("\n");
	}
	else if (node->node_type == NODE_PIPE)
		printf("PIPE\n");
	else if (node->node_type == NODE_REDIR)
	{
		if (node->redir_type == 0)
			redir = "REDIR_OUT";
		else if (node->redir_type == 1)
			redir = "REDIR_IN";
		else if (node->redir_type == 2)
			redir = "REDIR_APPEND";
		else
			redir = "REDIR_HEREDOC";
		printf("%s %s\n", redir, node->file);
	}
	print_ast(node->left_node, depth + 1);
	print_ast(node->right_node, depth + 1);
}

// void	print_argv(char *argv)
// {
// 	int i = 0;
// 	while (argv[i])
// 	{
// 		printf("%c", argv[i]);
// 		i++;
// 	}
// 	printf("\n");
// }