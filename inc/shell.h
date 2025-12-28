/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:29:27 by kwrzosek          #+#    #+#             */
/*   Updated: 2025/12/28 14:08:08 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
#define SHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../../libft/libft.h"
# include "parsing.h"

typedef struct s_env
{
	char	*key;
	char	*val;
	struct	s_env	*next;
	
}	t_env;

// exec.c
int	order_66(t_ast *root);

// exec_utils.c
char	*get_path(char *cmd, char **env);
char	*ft_getenv(char *name, char **env);

// builtins.c
int	identify_builtins(t_ast *tree);

//	env.c
t_env	*init_env(char **env);
t_env	*create_node(char **env, int i);
// char	*get_key(char **env, int i);
#endif