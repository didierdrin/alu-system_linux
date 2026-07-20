#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * send_response - send an HTTP response with optional body
 * @client_fd: connected client socket
 * @status: HTTP status code
 * @phrase: HTTP status phrase
 * @content_type: Content-Type header value, or NULL
 * @body: response body, or NULL
 */
void send_response(int client_fd, int status, const char *phrase,
		const char *content_type, const char *body)
{
	char header[BUFFER_SIZE];
	int body_length;
	int header_length;

	body_length = (body != NULL) ? (int)strlen(body) : 0;
	if (content_type != NULL)
	{
		header_length = snprintf(header, sizeof(header),
				"HTTP/1.1 %d %s\r\nContent-Length: %d\r\n"
				"Content-Type: %s\r\n\r\n",
				status, phrase, body_length, content_type);
	}
	else
	{
		header_length = snprintf(header, sizeof(header),
				"HTTP/1.1 %d %s\r\n\r\n", status, phrase);
	}

	write(client_fd, header, (size_t)header_length);
	if (body != NULL && body_length > 0)
		write(client_fd, body, (size_t)body_length);
}

/**
 * send_simple_response - send an HTTP response without a body
 * @client_fd: connected client socket
 * @status: HTTP status code
 * @phrase: HTTP status phrase
 */
void send_simple_response(int client_fd, int status, const char *phrase)
{
	send_response(client_fd, status, phrase, NULL, NULL);
}
