/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:29:27 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/10/23 19:28:04 by kwrzosek         ###   ########.fr       */
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

// need to find better way to pass this, but it will have to do for now
typedef struct s_globals
{
	int	token_size;
}	t_globals;

typedef struct s_token
{
	t_token_type	type;
	char			*val;
	int				length;
	struct s_token		*next;
}	t_token;


/* FUNCTIONS */
//	lexer.c
int	return_token(char *input, t_token **list);
void	emit_token(t_token **list, char *input, int i);
int	read_input (char *input);
t_token	*end_of_list(t_token **token);

//	lexer_utils.c
void	pick_handler(char *input, int i, t_token *token);
void	handle_general(char *input, t_token *token);
void	handle_squotes();
void	handle_dquotes();
void	handle_red_in(char *input, t_token *token);
void	handle_red_out(char *input, t_token *token);
void	handle_pipe(char *input, t_token *token);



void	print_token(t_token *token);

#endif