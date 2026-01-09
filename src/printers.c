/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:04:33 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 14:50:25 by sjesione         ###   ########.fr       */
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

static void	print_cmd_args(t_strlist *arg)
{
	printf("CMD: ");
	while (arg)
	{
		printf(" \"%s\"", arg->str);
		arg = arg->next;
	}
	printf("\n");
}

static char	*get_redir_label(int type)
{
	if (type == 0)
		return ("REDIR_OUT");
	if (type == 1)
		return ("REDIR_IN");
	if (type == 2)
		return ("REDIR_APPEND");
	return ("REDIR_HEREDOC");
}

void	print_ast(t_ast *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = -1;
	while (++i < depth)
		printf("  ");
	if (node->node_type == NODE_CMD)
		print_cmd_args(node->argv);
	else if (node->node_type == NODE_PIPE)
		printf("PIPE\n");
	else if (node->node_type == NODE_REDIR)
		printf("%s %s\n", get_redir_label(node->redir_type), node->file);
	print_ast(node->left_node, depth + 1);
	print_ast(node->right_node, depth + 1);
}
