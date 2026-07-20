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
 * parse_body_params - parse URL-encoded body parameters
 * @request: request structure to populate
 */
void parse_body_params(http_request_t *request)
{
	char *start;
	char *copy;

	start = strstr(request->raw, "\r\n\r\n");
	if (start == NULL)
		return;

	start += 4;
	if (*start == '\0')
		return;

	request->body = duplicate_string(start);
	copy = duplicate_string(start);
	if (copy == NULL)
		return;

	split_kv_pairs(request->body_params, &request->body_param_count, copy, '&');
	free(copy);
}

/**
 * get_body_param - retrieve a body parameter value by name
 * @request: parsed HTTP request
 * @name: parameter name
 *
 * Return: parameter value, or NULL if not found
 */
char *get_body_param(http_request_t *request, const char *name)
{
	int index;

	for (index = 0; index < request->body_param_count; index++)
	{
		if (strcmp(request->body_params[index].key, name) == 0)
			return (request->body_params[index].value);
	}
	return (NULL);
}
