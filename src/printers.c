/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:04:33 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/11/19 18:48:52 by kwrzosek         ###   ########.fr       */
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

// --- Indentation ---
// void print_indent(int indent_level)
// {
//     for (int i = 0; i < indent_level; i++)
//         printf("|   ");
// }

// // --- Node Type to String Mapping ---
// char *get_node_type_str(int type)
// {
//     if (type == NODE_CMD)
//         return "COMMAND_NODE";
//     else if (type == NODE_PIPE)
//         return "PIPE_NODE";
//     else if (type == NODE_REDIR)
//         return "REDIR_NODE";
//     else
//         return "UNKNOWN_NODE";
// }

// // --- Redirection Type to Symbol Mapping ---
// char *get_redir_symbol(int type)
// {
//     if (type == TOKEN_RED_IN)
//         return "<";
//     else if (type == TOKEN_RED_OUT)
//         return ">";
//     else if (type == TOKEN_APPEND)
//         return ">>";
//     else if (type == TOKEN_HEREDOC)
//         return "<<";
//     else
//         return "?";
// }

// void print_ast(t_ast *node, int indent_level)
// {
//     if (node == NULL)
//         return;

//     // 1. Traverse and print the RIGHT child first (to match common tree visualization)
//     print_ast(node->right_node, indent_level + 1);

//     // 2. Print the current node's information with indentation
//     print_indent(indent_level);
//     printf("--- %s (%p) ---\n", get_node_type_str(node->node_type), (void *)node);

//     // 3. Print node-specific details
//     print_indent(indent_level);
//     if (node->node_type == NODE_CMD)
//     {
//         t_strlist *current_arg = node->argv;
//         printf("Args: [");
//         while (current_arg != NULL)
//         {
//             printf(" \"%s\"", current_arg->str);
//             current_arg = current_arg->next;
//         }
//         printf(" ]\n");
//     }
//     else if (node->node_type == NODE_REDIR)
//     {
//         printf("Redir: %s File: \"%s\"\n", 
//                get_redir_symbol(node->redir_type), 
//                node->file ? node->file : "(null)");
//     }
    
//     // 4. Traverse and print the LEFT child
//     print_ast(node->left_node, indent_level + 1);
// }

void    print_ast(t_ast *node, int depth)
{
    if (!node)
        return ;
    t_strlist *current_arg = node->argv;
    int i;
    
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
        printf("REDIR %s\n", node->file);
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