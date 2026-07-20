#include "server.h"
#include "http.h"
#include "handler.h"
#include "todo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#ifndef API_LEVEL
#define API_LEVEL 0
#endif

/**
 * process_client - handle a single client connection
 * @client_fd: connected client socket
 * @client_ip: client IP address
 */
static void process_client(int client_fd, const char *client_ip)
{
	http_request_t request;
	char *raw;

	printf("Client connected: %s\n", client_ip);
	raw = receive_request(client_fd);
	if (raw == NULL)
	{
		close(client_fd);
		return;
	}

	memset(&request, 0, sizeof(request));
	request.raw = raw;
	print_raw_request(raw);

#if API_LEVEL >= 0
	parse_request_line(&request);
#if API_LEVEL == 0
	print_request_line(&request);
#endif
#endif

#if API_LEVEL >= 1
#if API_LEVEL == 1
	print_queries(&request);
#endif
#endif

#if API_LEVEL >= 2
	parse_headers(&request);
#if API_LEVEL == 2
	print_headers(&request);
#endif
#endif

#if API_LEVEL == 3
	parse_body_params(&request);
	print_body_params(&request);
#endif
#if API_LEVEL >= 4
	handle_api_request(&request, client_fd, client_ip, API_LEVEL);
#else
	send_simple_response(client_fd, 200, "OK");
#endif

	free_request(&request);
	close(client_fd);
}

/**
 * main - HTTP server entry point
 *
 * Return: 0 on success, EXIT_FAILURE on error
 */
int main(void)
{
	int server_fd;
	int client_fd;
	struct sockaddr_in client_addr;
	char client_ip[INET_ADDRSTRLEN];

#if API_LEVEL >= 4
	todos_init();
#endif

	server_fd = create_server_socket(8080);
	if (server_fd < 0)
	{
		perror("server");
		return (EXIT_FAILURE);
	}

	printf("Server listening on port 8080\n");
	fflush(stdout);

	while (1)
	{
		client_fd = accept_client(server_fd, &client_addr,
				client_ip, sizeof(client_ip));
		if (client_fd < 0)
			continue;

		process_client(client_fd, client_ip);
	}

	close(server_fd);
#if API_LEVEL >= 4
	todos_free();
#endif
	return (0);
}
