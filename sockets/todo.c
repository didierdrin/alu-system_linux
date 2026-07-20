#include "todo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static todo_t **todo_list;
static int todo_count;
static int next_id;

/**
 * todos_init - initialize the in-memory todo list
 */
void todos_init(void)
{
	todo_list = NULL;
	todo_count = 0;
	next_id = 0;
}

/**
 * todos_free - free all todos from memory
 */
void todos_free(void)
{
	int index;

	for (index = 0; index < todo_count; index++)
	{
		free(todo_list[index]->title);
		free(todo_list[index]->description);
		free(todo_list[index]);
	}
	free(todo_list);
	todo_list = NULL;
	todo_count = 0;
	next_id = 0;
}

/**
 * todos_create - create and store a new todo
 * @title: todo title
 * @description: todo description
 * @created: output pointer to created todo
 *
 * Return: 0 on success, -1 on failure
 */
int todos_create(const char *title, const char *description, todo_t **created)
{
	todo_t *todo;
	todo_t **updated;

	todo = malloc(sizeof(todo_t));
	if (todo == NULL)
		return (-1);

	todo->id = next_id++;
	todo->title = strdup(title);
	todo->description = strdup(description);
	if (todo->title == NULL || todo->description == NULL)
	{
		free(todo->title);
		free(todo->description);
		free(todo);
		return (-1);
	}

	updated = realloc(todo_list, sizeof(todo_t *) * (todo_count + 1));
	if (updated == NULL)
	{
		free(todo->title);
		free(todo->description);
		free(todo);
		return (-1);
	}

	todo_list = updated;
	todo_list[todo_count] = todo;
	todo_count++;
	*created = todo;
	return (0);
}

/**
 * todos_get_all - retrieve all todos
 * @list: output pointer to todo array
 * @count: output number of todos
 *
 * Return: 0 on success
 */
int todos_get_all(todo_t ***list, int *count)
{
	*list = todo_list;
	*count = todo_count;
	return (0);
}
