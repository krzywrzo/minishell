/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:29:27 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/02 15:23:24 by kwrzosek         ###   ########.fr       */
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

int	g_exit_status;

// exec.c
int	order_66(t_ast *root, t_env *env);
char	**convert_list_to_arr(t_env *env);
int	list_size(t_env *env);
char *join_env_str(char *key, char *val);
int	handle_error();
int	is_state_changing(char *cmd);
void	fork_and_run(char **cmd, char *cmd_path, char **env);
int	is_builtin(char **cmd);
int is_absolute_relative(char *cmd);
void    run_non_standard(char **argv, char **envp);

int	exec_cmd(t_ast *node, t_env *env, int is_piped);
int	exec_redir();
int	exec_pipe(t_ast *node, t_env *env);

// exec_utils.c
char	*get_path(char *cmd, char **env);
char	*ft_getenv(char *name, char **env);

// builtins.c
// int	identify_builtins(t_ast *tree);
int	identify_builtins(char **cmd, char *cmd_path, char **env_arr);
int echo_builtin();
int cd_builtin();
int pwd_builtin();
int unset_builtin();
int env_builtin();
int exit_builtin();
int	export_builtin();

//	env.c
t_env	*init_env(char **env);
t_env	*create_node(char **env, int i);
// char	*get_key(char **env, int i);
#endif