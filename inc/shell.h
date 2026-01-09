/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:22:36 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/09 15:22:37 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "../libft/libft.h"
# include "parsing.h"

typedef struct s_env
{
	char			*key;
	char			*val;
	struct s_env	*next;
}	t_env;

extern int	g_exit_status;

// exec.c
int		order_66(t_ast *root, t_env *env);
char	**convert_list_to_arr(t_env *env);
int		list_size(t_env *env);
char	*join_env_str(char *key, char *val);
int		handle_error(void);
int		is_state_changing(char *cmd);
void	fork_and_run(t_ast *node, t_env *env);
int		is_builtin(char *cmd);
int		is_absolute_relative(char *cmd);
void	run_non_standard(char **argv, char **envp);
int		what_fd(t_ast *node);
int		exec_cmd(t_ast *node, t_env *env, int is_piped);
int		exec_redir(t_ast *node, t_env *env);
int		exec_pipe(t_ast *node, t_env *env);

// exec_utils.c
char	*get_path(char *cmd, char **env);
char	*ft_getenv(char *name, char **env);
void	err_putstr(char *cmd);
void	child_signals(void);

// builtins.c
int		identify_builtins(t_ast *node, t_env *env);
int		echo_builtin(char **argv);
int		cd_builtin(char **argv, t_env *env);
int		pwd_builtin(void);
int		export_builtin(char **argv, t_env *env);
int		unset_builtin(char **argv, t_env *env);
int		env_builtin(t_env *env);
int		exit_builtin(char **argv, t_env *env);
void	update_wd_env(t_env *env, char *key, char *new_val);
int		is_valid_key(char *key);
char	*get_env_val(t_env *env, char *key);

// env.c
t_env	*init_env(char **env);
t_env	*create_node(char **env, int i);
void	free_env(t_env *env);
void	delete_node(t_env **head, char *key);
void	add_or_update_env(t_env **head, char *key, char *val);

#endif