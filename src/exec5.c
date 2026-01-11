/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjesione < sjesione@student.42warsaw.pl    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 14:42:49 by sjesione          #+#    #+#             */
/*   Updated: 2026/01/11 17:41:48 by sjesione         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/shell.h"

static char	*read_line_from_stdin_fgets(void)
{
	char	buffer[4096];
	size_t	len;

	if (fgets(buffer, sizeof(buffer), stdin) == NULL)
		return (NULL);
	len = ft_strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n')
		buffer[len - 1] = '\0';
	return (ft_strdup(buffer));
}

static char	*expand_and_join(char *content, char *line, t_env *env)
{
	char	*expanded_line;
	char	*temp;

	expanded_line = expand_variables(line, env);
	free(line);
	temp = content;
	content = ft_strjoin(content, expanded_line);
	free(temp);
	free(expanded_line);
	temp = content;
	content = ft_strjoin(content, "\n");
	free(temp);
	return (content);
}

static int	is_end_delim(char *line, char *delim)
{
	if (ft_strncmp(line, delim, ft_strlen(delim)) == 0)
		if (ft_strlen(line) == ft_strlen(delim))
			return (1);
	return (0);
}

static char	*get_next_heredoc_line(int is_interactive)
{
	if (is_interactive)
		return (readline("> "));
	return (read_line_from_stdin_fgets());
}

char	*read_heredoc_content(char *delimiter, t_env *env)
{
	char	*line;
	char	*content;
	int		is_interactive;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		line = get_next_heredoc_line(is_interactive);
		if (!line)
			break ;
		if (is_end_delim(line, delimiter))
		{
			free(line);
			break ;
		}
		content = expand_and_join(content, line, env);
	}
	return (content);
}
