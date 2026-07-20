#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * create_server_socket - create and bind a listening TCP socket
 * @port: port number to listen on
 *
 * Return: server socket file descriptor, or -1 on failure
 */
int create_server_socket(unsigned short port)
{
	int server_fd;
	struct sockaddr_in server_addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		return (-1);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		close(server_fd);
		return (-1);
	}

	if (listen(server_fd, 10) < 0)
	{
		close(server_fd);
		return (-1);
	}

	return (server_fd);
}

/**
 * accept_client - accept an incoming client connection
 * @server_fd: listening socket file descriptor
 * @client_addr: output client address structure
 * @client_ip: buffer for client IP string
 * @ip_size: size of client_ip buffer
 *
 * Return: client socket file descriptor, or -1 on failure
 */
int accept_client(int server_fd, struct sockaddr_in *client_addr,
		char *client_ip, size_t ip_size)
{
	int client_fd;
	socklen_t client_len;

	client_len = sizeof(*client_addr);
	client_fd = accept(server_fd, (struct sockaddr *)client_addr, &client_len);
	if (client_fd < 0)
		return (-1);

	inet_ntop(AF_INET, &client_addr->sin_addr, client_ip, ip_size);
	return (client_fd);
}
