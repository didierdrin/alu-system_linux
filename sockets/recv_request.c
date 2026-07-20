#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

/**
 * find_header_end - locate the end of HTTP headers in a buffer
 * @buffer: request buffer
 *
 * Return: index after header terminator, or -1 if not found
 */
static int find_header_end(const char *buffer)
{
	char *end;

	end = strstr(buffer, "\r\n\r\n");
	if (end == NULL)
		return (-1);

	return ((int)(end - buffer) + 4);
}

/**
 * read_content_length - extract Content-Length from request headers
 * @buffer: request buffer
 *
 * Return: content length value, or 0 if not found
 */
static int read_content_length(const char *buffer)
{
	const char *key = "Content-Length:";
	char *start;
	char *end;
	int length;

	start = strstr(buffer, key);
	if (start == NULL)
		return (0);

	start += strlen(key);
	while (*start == ' ')
		start++;
	length = (int)strtol(start, &end, 10);
	return (length);
}

/**
 * receive_request - read a full HTTP request from a client socket
 * @client_fd: connected client socket
 *
 * Return: dynamically allocated request string, or NULL on failure
 */
char *receive_request(int client_fd)
{
	char buffer[BUFFER_SIZE];
	char *request;
	char *tmp;
	int read_total;
	int total;
	int header_end;
	int content_length;
	int needed;
	int bytes;
	int in_buffer;
	int to_copy;

	read_total = 0;
	header_end = -1;
	memset(buffer, 0, sizeof(buffer));

	while (read_total < (int)sizeof(buffer) - 1)
	{
		bytes = recv(client_fd, buffer + read_total,
				sizeof(buffer) - 1 - read_total, 0);
		if (bytes <= 0)
			return (NULL);

		read_total += bytes;
		buffer[read_total] = '\0';
		header_end = find_header_end(buffer);
		if (header_end >= 0)
			break;
	}

	if (header_end < 0)
		return (NULL);

	content_length = read_content_length(buffer);
	needed = header_end + content_length;
	request = malloc(needed + 1);
	if (request == NULL)
		return (NULL);

	memcpy(request, buffer, (size_t)header_end);
	total = header_end;

	if (read_total > header_end)
	{
		in_buffer = read_total - header_end;
		to_copy = in_buffer;
		if (to_copy > content_length)
			to_copy = content_length;
		memcpy(request + total, buffer + header_end, (size_t)to_copy);
		total += to_copy;
	}

	while (total < needed)
	{
		bytes = recv(client_fd, request + total, needed - total, 0);
		if (bytes <= 0)
		{
			free(request);
			return (NULL);
		}
		total += bytes;
	}

	request[needed] = '\0';
	tmp = realloc(request, (size_t)needed + 1);
	if (tmp != NULL)
		request = tmp;

	return (request);
}
