/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaigonza <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:33:08 by jaigonza          #+#    #+#             */
/*   Updated: 2024/05/31 12:25:02 by jaigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_read(int fd, char *buf, char **buffer)
{
	char	*temp;
	int		readbytes;
	size_t	buf_size;

	buf_size = BUFFER_SIZE;
	readbytes = 1;
	if (gnl_strchr(*buffer, '\n'))
		return ;
	while (readbytes > 0)
	{
		readbytes = read(fd, buf, buf_size);
		if (readbytes == -1 || readbytes == 0)
			return ;
		buf[readbytes] = '\0';
		temp = ft_strjoin(*buffer, buf);
		free(*buffer);
		*buffer = NULL;
		*buffer = temp;
		if (gnl_strchr(buf, '\n'))
			break ;
	}
}

char	*ft_read_text(int fd, char *buffer)
{
	char	*buf;
	size_t	buf_size;

	buf_size = BUFFER_SIZE;
	buf = (char *)malloc((buf_size + 1) * sizeof(char));
	if (!buf)
	{
		free(buffer);
		return (NULL);
	}
	ft_read(fd, buf, &buffer);
	free(buf);
	if (buffer && buffer[0] == '\0')
	{
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

char	*ft_get_line(char *buffer)
{
	char	*line;
	int		i;

	if (!buffer)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	line = (char *)malloc((i + 1) * sizeof(char));
	if (!line)
	{
		free(buffer);
		return (NULL);
	}
	ft_strlcpy(line, buffer, (i + 1));
	return (line);
}

char	*ft_next_line(char **buffer)
{
	int		i;
	char	*rest;

	i = 0;
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	if (!(*buffer)[i])
	{
		free(*buffer);
		*buffer = NULL;
		return (NULL);
	}
	rest = (char *)malloc((gnl_strlen(*buffer) - i + 1) * sizeof(char));
	if (!rest)
	{
		free(*buffer);
		*buffer = NULL;
		return (NULL);
	}
	i++;
	ft_strlcpy(rest, &((*buffer)[i]), gnl_strlen(*buffer) - i + 1);
	free(*buffer);
	*buffer = rest;
	return (*buffer);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	buffer = initialize_buffer(buffer);
	if (!buffer)
		return (NULL);
	buffer = ft_read_text(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_get_line(buffer);
	if (!line)
	{
		buffer = NULL;
		return (NULL);
	}
	buffer = ft_next_line(&buffer);
	return (line);
}
/*int main(void)
{
    int fd;
    char    *next_line;
    fd = open("a.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error");
        return (1);
    }

    while ((next_line = get_next_line(fd)) != NULL)
    {
        printf("%s\n", next_line);
        free(next_line);
        next_line = NULL;
    }

    close(fd);
    return (0);
}*/
