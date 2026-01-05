/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwrzosek <kwrzosek@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:45:25 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/05 16:06:40 by kwrzosek         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

int	order_66(t_ast *root, t_env *env)
{
	if (!root)
		return (0);
	if (root->node_type == NODE_PIPE)
	{
		if(exec_pipe(root, env) == -1)
			return(handle_error());
	}
	else if (root->node_type == NODE_REDIR)
	{
		if (exec_redir(root, env) == -1)
			return(handle_error());
	}
	else
	{
		if (exec_cmd(root, env, 0) == -1)
			return(handle_error());
	}
	return(0);
}

int exec_pipe(t_ast *node, t_env *env)
{
    int     fd[2];
    pid_t   pid_left;
    pid_t   pid_right;
    int     status;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return (1);
    }
    pid_left = fork();
    if (pid_left == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return (1);
    }
    if (pid_left == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        exec_cmd(node->left_node, env, 1); 
		// order_66(node->left_node, env);
        exit(g_exit_status);
    }
    pid_right = fork();
    if (pid_right == -1)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return (1);
    }
    if (pid_right == 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        exec_cmd(node->right_node, env, 1);
		// order_66(node->right_node, env);
        exit(g_exit_status);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid_left, NULL, 0);
    waitpid(pid_right, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    return (0);
}

int	what_fd(t_ast *node)
{
	int fd;

	if (node->redir_type == REDIR_IN)
		fd = open(node->file, O_RDONLY);
	else if (node->redir_type == REDIR_OUT)
		fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->redir_type == REDIR_APPEND)
		fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	restore_fd(t_ast *node, int original_fd)
{
	if (node->redir_type == REDIR_IN)
        return (dup2(original_fd, STDIN_FILENO));
    else
        return(dup2(original_fd, STDOUT_FILENO));
}

int	exec_redir(t_ast *node, t_env *env)
{
	int fd;
	int	original_fd;

	fd = what_fd(node);
	if (fd == -1)
	{
		perror("minishell");
		g_exit_status = 1;
		return(-1);
	}
	if (node->redir_type == REDIR_IN)
        original_fd = dup(STDIN_FILENO);
    else
        original_fd = dup(STDOUT_FILENO);
	if (node->redir_type == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	order_66(node->left_node, env);
	restore_fd(node, original_fd);
    close(original_fd);
	return (0);
}

int exec_cmd(t_ast *node, t_env *env, int is_piped)
{
    if (!node->argv || !node->argv->str)
        return (0);
	if (is_builtin(node->argv->str) == 1)
    {
        if (is_piped == 0 && is_state_changing(node->argv->str))
        {
            identify_builtins(node, env);
            return (0);
        }
        else
        {
            identify_builtins(node, env);
            return (0);
        }
    }
    fork_and_run(node, env);
    return (0);
}

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strncmp(cmd, "echo", 5) == 0)
        return (1);
    else if (ft_strncmp(cmd, "cd", 3) == 0)
        return (1);
    else if (ft_strncmp(cmd, "pwd", 4) == 0)
        return (1);
    else if (ft_strncmp(cmd, "export", 7) == 0)
        return (1);
    else if (ft_strncmp(cmd, "unset", 6) == 0)
        return (1);
    else if (ft_strncmp(cmd, "env", 4) == 0)
        return (1);
    else if (ft_strncmp(cmd, "exit", 5) == 0)
        return (1);
    else
	    return (0);
}

static void err_putstr(char *cmd)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": command not found\n", 2);
}
void child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void handle_sigint(int sig)
{
    (void)sig;
    
    ft_putchar_fd('\n', 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_exit_status = 1;
}

void setup_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
}

void    fork_and_run(t_ast *node, t_env *env)
{
    pid_t   pid;
    int     status;
    char    **cmd;
    char    *cmd_path;
    char    **env_arr;

	//	TODO: signal handling
    if (!node->argv || !node->argv->str)
        return ;
    env_arr = convert_list_to_arr(env);
    if (!env_arr)
        return ;  

    cmd = list_to_argv(node->argv);
    if (!cmd)
    {
        free_ast_argv(env_arr);
        return ;
    }
    cmd_path = NULL;
    if (cmd[0] && !is_builtin(cmd[0]) && !is_absolute_relative(cmd[0]))
        cmd_path = get_path(cmd[0], env_arr);
	signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        free_ast_argv(env_arr);
        free_ast_argv(cmd);
        if (cmd_path) free(cmd_path);
        return ;
    }
    if (pid == 0)
    {
		child_signals();
        if (cmd[0] && is_builtin(cmd[0]) == 1) 
        {
            identify_builtins(node, env);
            exit(0);
        }
        if (cmd[0] && is_absolute_relative(cmd[0]) == 1)
            run_non_standard(cmd, env_arr);
        if (cmd_path)
            execve(cmd_path, cmd, env_arr);
            
        if (cmd[0])
            err_putstr(cmd[0]);
        exit(127);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
        {
            g_exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGQUIT)
                ft_putstr_fd("Quit: 3\n", 1);
            else if (WTERMSIG(status) == SIGINT)
                ft_putstr_fd("\n", 1);
        }
		signal(SIGINT, handle_sigint);
        free_ast_argv(cmd);
        free_ast_argv(env_arr);
        if (cmd_path)
            free(cmd_path);
    }
}

void    run_non_standard(char **argv, char **envp)
{
    char        *path;

    path = argv[0];
    if (access(path, F_OK) == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        exit(127);
    }
    if (access(path, X_OK) == -1)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": Permission denied\n", 2);
        exit(126);
    }
    execve(path, argv, envp);
    perror("execve");
    exit(1);
}

int is_absolute_relative(char *cmd)
{
    if (!cmd || !cmd[0])
        return (0);
    if (ft_strchr(cmd, '/'))
        return (1);

    return (0);
}


int	is_state_changing(char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (1);
	return (0);
}

int	handle_error()	//	TODO: handle execve error
{
	return(-1);
}

char *join_env_str(char *key, char *val)
{
    char    *tmp;
    char    *res;

    if (!key || !val)
        return (NULL);
    tmp = ft_strjoin(key, "=");
    if (!tmp)
        return (NULL);
    res = ft_strjoin(tmp, val);
    free(tmp);
    return (res);
}

char    **convert_list_to_arr(t_env *env)
{
    char    **env_arr;
    int     env_size;
    int     i;

    env_size = list_size(env);
    env_arr = ft_calloc(env_size + 1, sizeof(char *));
    if (!env_arr)
        return (NULL);
    i = 0;
    while (env)
    {
        if (env->val) 
        {
            env_arr[i] = join_env_str(env->key, env->val);
            if (env_arr[i] == NULL) 
            {
                free_ast_argv(env_arr);
                return (NULL);
            }
            i++;
        }
        env = env->next;
    }
    return (env_arr);
}

int	list_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}
