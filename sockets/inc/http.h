#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

#define BUFFER_SIZE 8192
#define MAX_HEADERS 32
#define MAX_KV 32

/**
 * struct kv_s - key/value pair
 * @key: parameter or header name
 * @value: parameter or header value
 */
typedef struct kv_s
{
	char *key;
	char *value;
} kv_t;

/**
 * struct http_request_s - parsed HTTP request
 * @method: HTTP method
 * @path: request path
 * @version: HTTP version
 * @raw: full raw request
 * @queries: query string key/value pairs
 * @query_count: number of query pairs
 * @headers: request headers
 * @header_count: number of headers
 * @body: request body
 * @body_params: body key/value pairs
 * @body_param_count: number of body parameters
 */
typedef struct http_request_s
{
	char *method;
	char *path;
	char *version;
	char *raw;
	kv_t queries[MAX_KV];
	int query_count;
	kv_t headers[MAX_HEADERS];
	int header_count;
	char *body;
	kv_t body_params[MAX_KV];
	int body_param_count;
} http_request_t;

char *receive_request(int client_fd);
void free_request(http_request_t *request);
void parse_request_line(http_request_t *request);
void parse_queries(http_request_t *request, char *target);
void parse_headers(http_request_t *request);
void parse_body_params(http_request_t *request);
char *get_body_param(http_request_t *request, const char *name);
char *get_header_value(http_request_t *request, const char *name);
void print_raw_request(const char *raw);
void print_request_line(http_request_t *request);
void print_queries(http_request_t *request);
void print_headers(http_request_t *request);
void print_body_params(http_request_t *request);
void send_response(int client_fd, int status, const char *phrase,
		const char *content_type, const char *body);
void send_simple_response(int client_fd, int status, const char *phrase);

#endif
