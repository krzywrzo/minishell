/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec9.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:57 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static int	apply_heredoc_redir(t_ast *redir_node, t_heredoc *hd)
{
	int	fd;
	int	original_fd;

	fd = create_heredoc_tempfile(redir_node->file, hd);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: heredoc error\n", 2);
		g_exit_status = 1;
		return (-1);
	}
	original_fd = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (original_fd);
}

static int	apply_file_redir(t_ast *redir_node)
{
	int	fd;
	int	original_fd;

	fd = what_fd(redir_node);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir_node->file);
		g_exit_status = 1;
		return (-1);
	}
	if (redir_node->redir_type == REDIR_IN)
	{
		original_fd = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
	}
	else
	{
		original_fd = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
	}
	close(fd);
	return (original_fd);
}

int	apply_single_redir(t_ast *redir_node, t_heredoc *hd)
{
	if (redir_node->redir_type == REDIR_HEREDOC)
		return (apply_heredoc_redir(redir_node, hd));
	return (apply_file_redir(redir_node));
}

int	restore_single_redir(t_ast *redir_node, int original_fd)
{
	if (redir_node->redir_type == REDIR_IN
		|| redir_node->redir_type == REDIR_HEREDOC)
		return (dup2(original_fd, STDIN_FILENO));
	else
		return (dup2(original_fd, STDOUT_FILENO));
}
