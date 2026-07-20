#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * main - opens an IPv4/TCP socket and listens on port 12345
 *
 * Return: 0 on success, EXIT_FAILURE on error
 */
int main(void)
{
	int server_fd;
	struct sockaddr_in server_addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(12345);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		close(server_fd);
		return (EXIT_FAILURE);
	}

	if (listen(server_fd, 10) < 0)
	{
		perror("listen");
		close(server_fd);
		return (EXIT_FAILURE);
	}

	printf("Server listening on port 12345\n");
	fflush(stdout);

	while (1)
		pause();

	return (0);
}
