/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec8.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:00:00 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:54 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static char	*get_heredoc_tempfile_path(char *delimiter, t_heredoc *hd)
{
	int	i;

	i = 0;
	while (i < hd->count)
	{
		if (hd->delims[i]
			&& ft_strncmp(hd->delims[i], delimiter,
				ft_strlen(delimiter)) == 0
			&& ft_strlen(hd->delims[i]) == ft_strlen(delimiter))
			return (hd->temps[i]);
		i++;
	}
	return (NULL);
}

int	create_heredoc_tempfile(char *delimiter, t_heredoc *hd)
{
	char	*tmpfile_path;
	int		fd;

	tmpfile_path = get_heredoc_tempfile_path(delimiter, hd);
	if (!tmpfile_path)
		return (-1);
	fd = open(tmpfile_path, O_RDONLY);
	if (fd == -1)
	{
		perror("open heredoc tempfile");
		return (-1);
	}
	return (fd);
}

int	what_fd(t_ast *node)
{
	int		fd;
	char	*filename;
	char	*processed_filename;

	fd = -1;
	filename = node->file;
	processed_filename = process_string_with_quotes(filename, NULL);
	if (node->redir_type == REDIR_IN)
		fd = open(processed_filename, O_RDONLY);
	else if (node->redir_type == REDIR_OUT)
		fd = open(processed_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->redir_type == REDIR_APPEND)
		fd = open(processed_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (processed_filename != filename)
		free(processed_filename);
	return (fd);
}

int	restore_fd(t_ast *node, int original_fd)
{
	if (node->redir_type == REDIR_IN || node->redir_type == REDIR_HEREDOC)
		return (dup2(original_fd, STDIN_FILENO));
	else
		return (dup2(original_fd, STDOUT_FILENO));
}
