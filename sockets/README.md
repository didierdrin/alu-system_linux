# Sockets

This project covers socket programming in C and building a simple HTTP REST API
for managing an in-memory TODO list.

## Tasks

- `0-server.c` - Listen on port 12345
- `1-server.c` - Accept a client and print its IP address
- `2-client.c` - Connect to a remote server
- `3-server.c` - Receive and print a message from a client
- `todo_api_0` to `todo_api_5` - Progressive HTTP server and REST API

## Compilation

```bash
gcc -Wall -Wextra -Werror -pedantic -o 0-server 0-server.c
gcc -Wall -Wextra -Werror -pedantic -o 1-server 1-server.c
gcc -Wall -Wextra -Werror -pedantic -o 2-client 2-client.c
gcc -Wall -Wextra -Werror -pedantic -o 3-server 3-server.c
make todo_api_0
make todo_api_5
```

## REST API

- `POST /todos` - Create a todo (`title`, `description`)
- `GET /todos` - Retrieve all todos

## Author

ALU System Linux Project
