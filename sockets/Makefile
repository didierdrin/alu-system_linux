CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -Iinc

COMMON_SRC = server.c recv_request.c parse_request.c parse_headers.c \
	parse_body.c print_request.c response.c api_main.c

API4_SRC = handler.c todo.c todo_json.c
API5_SRC = handler.c todo.c todo_json.c

todo_api_0: CFLAGS += -DAPI_LEVEL=0
todo_api_0: $(COMMON_SRC)
	$(CC) $(CFLAGS) $^ -o $@

todo_api_1: CFLAGS += -DAPI_LEVEL=1
todo_api_1: $(COMMON_SRC)
	$(CC) $(CFLAGS) $^ -o $@

todo_api_2: CFLAGS += -DAPI_LEVEL=2
todo_api_2: $(COMMON_SRC)
	$(CC) $(CFLAGS) $^ -o $@

todo_api_3: CFLAGS += -DAPI_LEVEL=3
todo_api_3: $(COMMON_SRC)
	$(CC) $(CFLAGS) $^ -o $@

todo_api_4: CFLAGS += -DAPI_LEVEL=4
todo_api_4: $(COMMON_SRC) $(API4_SRC)
	$(CC) $(CFLAGS) $^ -o $@

todo_api_5: CFLAGS += -DAPI_LEVEL=5
todo_api_5: $(COMMON_SRC) $(API5_SRC)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -f todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5
