#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * split_kv_pairs - split key/value pairs from a string
 * @target: key/value array to fill
 * @count: output number of pairs
 * @source: source string
 * @pair_sep: pair separator character
 */
static void split_kv_pairs(kv_t *target, int *count, char *source, char pair_sep)
{
	char sep[2];
	char *pair;
	char *save_pair;
	char *value;
	int index;

	sep[0] = pair_sep;
	sep[1] = '\0';
	index = 0;
	pair = strtok_r(source, sep, &save_pair);
	while (pair != NULL && index < MAX_KV)
	{
		value = strchr(pair, '=');
		if (value != NULL)
		{
			*value = '\0';
			value++;
			target[index].key = duplicate_string(pair);
			target[index].value = duplicate_string(value);
			index++;
		}
		pair = strtok_r(NULL, sep, &save_pair);
	}
	*count = index;
}

/**
 * free_request - free memory used by a parsed HTTP request
 * @request: request structure to free
 */
void free_request(http_request_t *request)
{
	int index;

	if (request == NULL)
		return;

	free(request->method);
	free(request->path);
	free(request->version);
	free(request->raw);
	free(request->body);

	for (index = 0; index < request->query_count; index++)
	{
		free(request->queries[index].key);
		free(request->queries[index].value);
	}

	for (index = 0; index < request->header_count; index++)
	{
		free(request->headers[index].key);
		free(request->headers[index].value);
	}

	for (index = 0; index < request->body_param_count; index++)
	{
		free(request->body_params[index].key);
		free(request->body_params[index].value);
	}

	memset(request, 0, sizeof(*request));
}

/**
 * parse_request_line - parse method, path and version from a request
 * @request: request structure to populate
 */
void parse_request_line(http_request_t *request)
{
	char line_buffer[BUFFER_SIZE];
	char *line_end;
	char *target;
	char *version_start;
	char *query_start;
	size_t line_length;

	line_end = strstr(request->raw, "\r\n");
	if (line_end == NULL)
		return;

	line_length = (size_t)(line_end - request->raw);
	memcpy(line_buffer, request->raw, line_length);
	line_buffer[line_length] = '\0';

	target = strchr(line_buffer, ' ');
	if (target == NULL)
		return;
	*target = '\0';
	request->method = duplicate_string(line_buffer);
	target++;

	version_start = strchr(target, ' ');
	if (version_start == NULL)
		return;
	*version_start = '\0';
	query_start = strchr(target, '?');
	if (query_start != NULL)
	{
		*query_start = '\0';
		parse_queries(request, query_start + 1);
	}
	request->path = duplicate_string(target);
	version_start++;
	request->version = duplicate_string(version_start);
}

/**
 * parse_queries - parse query string key/value pairs
 * @request: request structure to populate
 * @target: query string without leading question mark
 */
void parse_queries(http_request_t *request, char *target)
{
	char *copy;

	copy = duplicate_string(target);
	if (copy == NULL)
		return;

	split_kv_pairs(request->queries, &request->query_count, copy, '&');
	free(copy);
}
