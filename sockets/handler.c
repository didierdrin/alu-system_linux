#include "http.h"
#include "todo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * handle_post_todos - handle POST /todos requests
 * @request: parsed HTTP request
 * @client_fd: connected client socket
 * @client_ip: client IP address
 *
 * Return: 0 on success
 */
static int handle_post_todos(http_request_t *request, int client_fd,
		const char *client_ip)
{
	todo_t *created;
	char *json;
	char *title;
	char *description;

	if (get_header_value(request, "Content-Length") == NULL)
	{
		printf("%s POST /todos -> 411 Length Required\n", client_ip);
		send_simple_response(client_fd, 411, "Length Required");
		return (0);
	}

	parse_body_params(request);
	title = get_body_param(request, "title");
	description = get_body_param(request, "description");
	if (title == NULL || description == NULL)
	{
		printf("%s POST /todos -> 422 Unprocessable Entity\n", client_ip);
		send_simple_response(client_fd, 422, "Unprocessable Entity");
		return (0);
	}

	if (todos_create(title, description, &created) != 0)
	{
		send_simple_response(client_fd, 500, "Internal Server Error");
		return (0);
	}

	printf("%s POST /todos -> 201 Created\n", client_ip);
	json = todo_to_json(created);
	send_response(client_fd, 201, "Created", "application/json", json);
	free(json);
	return (0);
}

/**
 * handle_get_todos - handle GET /todos requests
 * @client_fd: connected client socket
 *
 * Return: 0 on success
 */
static int handle_get_todos(int client_fd)
{
	todo_t **list;
	char *json;
	int count;

	todos_get_all(&list, &count);
	json = todos_to_json(list, count);
	send_response(client_fd, 200, "OK", "application/json", json);
	free(json);
	return (0);
}

/**
 * handle_api_request - route and handle REST API requests
 * @request: parsed HTTP request
 * @client_fd: connected client socket
 * @client_ip: client IP address
 * @api_level: API implementation level (4 or 5)
 *
 * Return: 0 on success
 */
int handle_api_request(http_request_t *request, int client_fd,
		const char *client_ip, int api_level)
{
	if (api_level >= 4 && strcmp(request->method, "POST") == 0 &&
		strcmp(request->path, "/todos") == 0)
		return (handle_post_todos(request, client_fd, client_ip));

	if (api_level >= 5 && strcmp(request->method, "GET") == 0 &&
		strcmp(request->path, "/todos") == 0)
	{
		printf("%s GET /todos -> 200 OK\n", client_ip);
		return (handle_get_todos(client_fd));
	}

	if (api_level >= 4)
	{
		printf("%s %s %s -> 404 Not Found\n",
			client_ip, request->method, request->path);
		send_simple_response(client_fd, 404, "Not Found");
		return (0);
	}

	send_simple_response(client_fd, 200, "OK");
	return (0);
}
