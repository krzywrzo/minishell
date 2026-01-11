/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:32 by sjesione         ###   ########.fr       */
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

static void	child_left_proc(t_ast *node, t_env *env, int *fd, t_heredoc *hd)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	order_66(node->left_node, env, 1, hd);
	exit(g_exit_status);
}

static void	child_right_proc(t_ast *node, t_env *env, int *fd, t_heredoc *hd)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	order_66(node->right_node, env, 1, hd);
	exit(g_exit_status);
}

static void	wait_children(pid_t pid_l, pid_t pid_r)
{
	int	status;

	waitpid(pid_l, NULL, 0);
	waitpid(pid_r, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}

int	exec_pipe(t_ast *node, t_env *env, t_heredoc *hd)
{
	int		fd[2];
	pid_t	pid_l;
	pid_t	pid_r;

	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	pid_l = fork();
	if (pid_l == -1)
		return (perror("fork"), close(fd[0]), close(fd[1]), 1);
	if (pid_l == 0)
		child_left_proc(node, env, fd, hd);
	pid_r = fork();
	if (pid_r == -1)
		return (close(fd[0]), close(fd[1]), kill(pid_l, SIGTERM),
			waitpid(pid_l, NULL, 0), perror("fork"), 1);
	if (pid_r == 0)
		child_right_proc(node, env, fd, hd);
	close(fd[0]);
	close(fd[1]);
	wait_children(pid_l, pid_r);
	return (0);
}
