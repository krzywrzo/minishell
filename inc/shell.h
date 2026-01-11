/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:22:36 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
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

# define MAX_HEREDOCS 100

typedef struct s_heredoc
{
	char	*temps[MAX_HEREDOCS];
	char	*delims[MAX_HEREDOCS];
	int		count;
}	t_heredoc;

extern int	g_exit_status;

// exec.c
int		order_66(t_ast *root, t_env *env, int is_piped, t_heredoc *hd);
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
int		exec_redir(t_ast *node, t_env *env, int is_piped, t_heredoc *hd);
int		exec_pipe(t_ast *node, t_env *env, t_heredoc *hd);

// exec_utils.c
char	*get_path(char *cmd, char **env);
char	*ft_getenv(char *name, char **env);
void	err_putstr(char *cmd);
void	child_signals(void);
void	setup_signals(void);

// main_utils.c
char	*get_input(int is_interactive);

// expansion.c
char	*expand_variables(char *str, t_env *env);

// builtins.c
int		identify_builtins(t_ast *node, t_env *env, int is_piped);
int		cd_builtin(char **argv, t_env *env);
int		export_builtin(char **argv, t_env *env);
int		unset_builtin(char **argv, t_env *env);
int		exit_builtin(char **argv, t_env *env, int is_piped);
int		is_valid_key(char *key);
char	*get_env_val(t_env *env, char *key);
int		is_exit_signal(int status);
int		get_exit_code(int status);

// exec5.c (heredoc handling)
void	preread_heredocs(t_ast *tree, t_env *env, t_heredoc *hd);
void	cleanup_heredocs(t_heredoc *hd);
int		create_heredoc_tempfile(char *delimiter, t_heredoc *hd);

// quotes.c (quote handling)
char	*process_string_with_quotes(char *str, t_env *env);

// env.c
t_env	*init_env(char **env);
t_env	*create_node(char **env, int i);
void	free_env(t_env *env);
void	delete_node(t_env **head, char *key);
void	add_or_update_env(t_env **head, char *key, char *val);

#endif