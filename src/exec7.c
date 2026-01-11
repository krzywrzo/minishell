/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec7.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:52 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

char	*read_heredoc_content(char *delimiter, t_env *env);

static void	write_heredoc_file(char *path, char *content)
{
	int		fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free(path);
		free(content);
		return ;
	}
	write(fd, content, ft_strlen(content));
	close(fd);
	free(content);
}

static void	preread_single_heredoc(t_ast *node, t_env *env, t_heredoc *hd)
{
	char	*content;
	char	*tmpfile_path;
	char	*counter_str;

	if (hd->count >= MAX_HEREDOCS)
		return ;
	content = read_heredoc_content(node->file, env);
	if (!content)
		return ;
	counter_str = ft_itoa(hd->count);
	tmpfile_path = ft_strjoin("/tmp/minishell_heredoc_", counter_str);
	free(counter_str);
	if (!tmpfile_path)
	{
		free(content);
		return ;
	}
	write_heredoc_file(tmpfile_path, content);
	hd->temps[hd->count] = tmpfile_path;
	hd->delims[hd->count] = node->file;
	hd->count++;
}

static void	preread_recursive(t_ast *node, t_env *env, t_heredoc *hd)
{
	if (!node)
		return ;
	preread_recursive(node->left_node, env, hd);
	preread_recursive(node->right_node, env, hd);
	if (node->node_type == NODE_REDIR && node->redir_type == REDIR_HEREDOC)
		preread_single_heredoc(node, env, hd);
}

void	preread_heredocs(t_ast *tree, t_env *env, t_heredoc *hd)
{
	hd->count = 0;
	preread_recursive(tree, env, hd);
}

void	cleanup_heredocs(t_heredoc *hd)
{
	int	i;

	i = 0;
	while (i < hd->count)
	{
		if (hd->temps[i])
		{
			unlink(hd->temps[i]);
			free(hd->temps[i]);
			hd->temps[i] = NULL;
		}
		i++;
	}
	hd->count = 0;
}
