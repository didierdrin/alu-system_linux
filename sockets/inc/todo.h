#ifndef TODO_H
#define TODO_H

/**
 * struct todo_s - todo item
 * @id: unique identifier
 * @title: todo title
 * @description: todo description
 */
typedef struct todo_s
{
	int id;
	char *title;
	char *description;
} todo_t;

void todos_init(void);
void todos_free(void);
int todos_create(const char *title, const char *description, todo_t **created);
int todos_get_all(todo_t ***list, int *count);
char *todo_to_json(const todo_t *todo);
char *todos_to_json(todo_t **list, int count);

#endif
