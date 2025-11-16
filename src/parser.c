/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 12:28:49 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/11/16 17:57:41 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

static t_ast	*wrap_redir(t_ast *cmd, t_token *token)
{
	t_redir	redir_mode;
	char	*file;

	if (!token->next || token->next->type != TOKEN_WORD)
		return (NULL);
	redir_mode = token_to_mode(token);
	file = ft_strdup(token->next->val);
	return (create_redir_node(cmd, redir_mode, file));
}

void	list_append(t_strlist **list, char *s)
{
	t_strlist	*tmp;
	t_strlist	*new;

	new = malloc(sizeof(t_strlist));
	if (!new)
		return ;
	new->str = s;
	new->next = NULL;
	while (!(*list))
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

//	TODO: split to couple functions
t_ast	*parse_token(t_token *token)
{
	t_strlist	*argv;
	t_ast		*root;
	t_ast		*current;
	t_ast		*cmd;

	argv = NULL;
	root = NULL;
	current = NULL;

	while (token)
	{
		
		/* WORD */
        if (token->type == TOKEN_WORD)
            list_append(&argv, ft_strdup(token->val));

		/* REDIRS */
        else if (token->type >= TOKEN_RED_IN && token->type <= TOKEN_HEREDOC)
        {
            cmd = build_cmd_from_list(argv);
            argv = NULL;
            current = wrap_redir(cmd, token);
            token = token->next;  /* skip filename token */
        }

		/* PIPE */
        else if (token->type == TOKEN_PIPE)
        {
            if (!current)
                current = build_cmd_from_list(argv);
            argv = NULL;

            if (!root)
                root = current;
            else
                root = create_pipe_node(root, current);
            current = NULL;
        }
        token = token->next;
    }

	/* finalize last command */
	if (!current)
		current = build_cmd_from_list(argv);
	else if (argv)
	{
		t_ast *tail = build_cmd_from_list(argv);
		if (tail)
			current->left_node = tail;
	}

	/* if no pipe occurred, return single cmd */
	if (!root)
		return (current);

	/* otherwise connect last segment */
	return (create_pipe_node(root, current));
}
