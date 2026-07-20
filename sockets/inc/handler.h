#ifndef HANDLER_H
#define HANDLER_H

#include "http.h"

int handle_api_request(http_request_t *request, int client_fd,
		const char *client_ip, int api_level);

#endif
