/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:29:27 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/10 18:37:27 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
#define SHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"


/*	STRUCTS	*/
struct	t_token;

typedef enum e_lex_state
{
	STATE_GENERAL,
	STATE_SQUOTES,
	STATE_DQUOTES,
	STATE_PIPE,
	STATE_REDIR,
	STATE_END
}	t_lex_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_RED_IN,
	TOKEN_RED_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_STRING
}	t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR
}	t_node_type;

typedef enum e_redir
{
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir;

typedef struct s_token
{
	t_token_type	type;
	char			*val;
	int				length;
	struct s_token		*next;
}	t_token;

typedef struct s_ast
{
	t_node_type	node_type;
	t_redir		redir_type;
	struct s_strlist	*argv;
	char		*val;
	char		*file;		// file name for redir
	struct s_ast	*left_node;
	struct s_ast	*right_node;	
}	t_ast;

typedef struct s_strlist
{
	char				*str;
	struct s_strlist	*next;
}	t_strlist;


/* FUNCTIONS */

// lexer_utils.c
void	pick_handler(char *input, int i, t_token *token);
void	handle_general(char *input, t_token *token);
void	handle_red_out(char *input, t_token *token);
void	handle_red_in(char *input, t_token *token);
void	handle_pipe(t_token *token);
void	handle_squotes(char *input, t_token *token);
void	handle_dquotes(char *input, t_token *token);
int		ft_issign(char str);

// lexer.c
void	emit_token(t_token **list, char *input, int i);
t_token	*read_input(char *input);
t_token	*end_of_list(t_token **token);

// parser_utils.c
t_redir	token_to_mode(t_token *token);
t_ast	*create_pipe_node(t_ast *left, t_ast *right);
t_ast	*create_redir_node(t_ast *cmd, t_redir mode, char *file);
void	free_strlist(t_strlist *list);
t_ast	*build_cmd_from_list(t_strlist *list);
char	**list_to_argv(t_strlist *list);
t_ast	*create_cmd_node(char **argv);

// parser.c
static t_ast	*wrap_redir(t_ast *cmd, t_token *token);
void	list_append(t_strlist **list, char *s);
t_ast	*parse_token(t_token *tokens);

t_ast *merge_ast_nodes(t_ast *root_node, t_ast *args_node);
t_ast *find_base_command(t_ast *node);
void append_all_strings(t_strlist **destination_list, t_strlist *source_list);
t_strlist *find_list_tail(t_strlist *list);

// cleaners.c
void free_ast_argv(char **argv);
void	free_token(t_token *token);
void	free_argv(t_strlist *argv);
void	free_ast(t_ast *root);

// printers.c
void	print_token(t_token *token);
void	print_ast(t_ast *node, int depth);
void	print_argv(char *argv);

#endif