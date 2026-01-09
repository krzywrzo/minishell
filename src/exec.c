/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 17:45:25 by kwrzosek          #+#    #+#             */
/*   Updated: 2026/01/09 18:37:05 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static int	has_executable_cmd(t_ast *node)
{
	if (!node)
		return (0);
	if (node->node_type == NODE_CMD && node->argv && node->argv->str)
		return (1);
	if (node->left_node && has_executable_cmd(node->left_node))
		return (1);
	if (node->right_node && has_executable_cmd(node->right_node))
		return (1);
	return (0);
}

int	order_66(t_ast *root, t_env *env, int is_piped)
{
	if (!root)
		return (0);
	if (root->node_type == NODE_PIPE)
	{
		if (exec_pipe(root, env) == -1)
			return (handle_error());
	}
	else if (root->node_type == NODE_REDIR)
	{
		if (exec_redir(root, env, is_piped) == -1)
			return (handle_error());
	}
	else
	{
		if (exec_cmd(root, env, is_piped) == -1)
			return (handle_error());
	}
	return (0);
}

static void	child_left(t_ast *node, t_env *env, int *fd)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	order_66(node->left_node, env, 0);
	exit(g_exit_status);
}

static void	child_right(t_ast *node, t_env *env, int *fd, int is_piped)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	order_66(node->right_node, env, is_piped);
	exit(g_exit_status);
}

static int	parent_wait(pid_t pid_l, pid_t pid_r, int *fd)
{
	int	status;

	close(fd[0]);
	close(fd[1]);
	waitpid(pid_l, NULL, 0);
	waitpid(pid_r, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	return (0);
}

int	exec_pipe(t_ast *node, t_env *env)
{
	int	fd[2];
	pid_t	pid_l;
	pid_t	pid_r;
	int	has_output;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	pid_l = fork();
	if (pid_l == -1)
		return (perror("fork"), close(fd[0]), close(fd[1]), 1);
	if (pid_l == 0)
		child_left(node, env, fd);
	has_output = has_executable_cmd(node->left_node);
	pid_r = fork();
	if (pid_r == -1)
	{
		perror("fork");
		close(fd[0]);
		close(fd[1]);
		kill(pid_l, SIGTERM);
		waitpid(pid_l, NULL, 0);
		return (1);
	}
	if (pid_r == 0)
		child_right(node, env, fd, has_output);
	return (parent_wait(pid_l, pid_r, fd));
}
