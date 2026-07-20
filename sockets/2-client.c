#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/**
 * main - connects to a listening server
 * @argc: argument count
 * @argv: argument array
 *
 * Return: 0 on success, EXIT_FAILURE on error
 */
int main(int argc, char *argv[])
{
	int client_fd;
	struct sockaddr_in server_addr;
	struct hostent *host;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	host = gethostbyname(argv[1]);
	if (host == NULL)
	{
		perror("gethostbyname");
		return (EXIT_FAILURE);
	}

	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((unsigned short)atoi(argv[2]));
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);

	if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("connect");
		close(client_fd);
		return (EXIT_FAILURE);
	}

	printf("Connected to %s:%s\n", argv[1], argv[2]);
	close(client_fd);
	return (0);
}
