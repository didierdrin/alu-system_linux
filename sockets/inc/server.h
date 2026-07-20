#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

int create_server_socket(unsigned short port);
int accept_client(int server_fd, struct sockaddr_in *client_addr,
		char *client_ip, size_t ip_size);

#endif
