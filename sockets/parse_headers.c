#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/**
 * duplicate_string - duplicate a string
 * @src: source string
 *
 * Return: newly allocated copy, or NULL on failure
 */
static char *duplicate_string(const char *src)
{
	char *copy;
	size_t length;

	length = strlen(src);
	copy = malloc(length + 1);
	if (copy == NULL)
		return (NULL);

	memcpy(copy, src, length + 1);
	return (copy);
}

/**
 * trim_spaces - remove leading spaces from a string
 * @value: string to trim
 *
 * Return: trimmed string pointer
 */
static char *trim_spaces(char *value)
{
	while (*value == ' ')
		value++;
	return (value);
}

/**
 * parse_headers - parse HTTP headers from a request
 * @request: request structure to populate
 */
void parse_headers(http_request_t *request)
{
	char *start;
	char *end;
	char *line;
	char *separator;
	char line_buffer[BUFFER_SIZE];
	int index;

	start = strstr(request->raw, "\r\n");
	if (start == NULL)
		return;

	start += 2;
	end = strstr(start, "\r\n\r\n");
	if (end == NULL)
		return;

	index = 0;
	while (start < end && index < MAX_HEADERS)
	{
		line = line_buffer;
		while (start < end && *start != '\r')
			*line++ = *start++;
		*line = '\0';
		if (*start == '\r')
			start += 2;

		separator = strchr(line_buffer, ':');
		if (separator != NULL)
		{
			*separator = '\0';
			separator++;
			request->headers[index].key = duplicate_string(line_buffer);
			request->headers[index].value = duplicate_string(trim_spaces(separator));
			index++;
		}
	}
	request->header_count = index;
}

/**
 * get_header_value - retrieve a header value by name
 * @request: parsed HTTP request
 * @name: header name
 *
 * Return: header value, or NULL if not found
 */
char *get_header_value(http_request_t *request, const char *name)
{
	int index;

	for (index = 0; index < request->header_count; index++)
	{
		if (strcasecmp(request->headers[index].key, name) == 0)
			return (request->headers[index].value);
	}
	return (NULL);
}
