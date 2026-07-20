#include "http.h"

#include <stdio.h>

/**
 * print_raw_request - print the full raw HTTP request
 * @raw: raw request string
 */
void print_raw_request(const char *raw)
{
	printf("Raw request: \"%s\"\n", raw);
}

/**
 * print_request_line - print parsed request line fields
 * @request: parsed HTTP request
 */
void print_request_line(http_request_t *request)
{
	printf("Method: %s\n", request->method);
	printf("Path: %s\n", request->path);
	printf("Version: %s\n", request->version);
}

/**
 * print_queries - print query string key/value pairs
 * @request: parsed HTTP request
 */
void print_queries(http_request_t *request)
{
	int index;

	printf("Path: %s\n", request->path);
	for (index = 0; index < request->query_count; index++)
	{
		printf("Query: \"%s\" -> \"%s\"\n",
			request->queries[index].key,
			request->queries[index].value);
	}
}

/**
 * print_headers - print HTTP header key/value pairs
 * @request: parsed HTTP request
 */
void print_headers(http_request_t *request)
{
	int index;

	for (index = 0; index < request->header_count; index++)
	{
		printf("Header: \"%s\" -> \"%s\"\n",
			request->headers[index].key,
			request->headers[index].value);
	}
}

/**
 * print_body_params - print body parameter key/value pairs
 * @request: parsed HTTP request
 */
void print_body_params(http_request_t *request)
{
	int index;

	printf("Path: %s\n", request->path);
	for (index = 0; index < request->body_param_count; index++)
	{
		printf("Body param: \"%s\" -> \"%s\"\n",
			request->body_params[index].key,
			request->body_params[index].value);
	}
}
