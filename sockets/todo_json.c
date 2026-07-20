#include "todo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * todo_to_json - serialize a todo to JSON
 * @todo: todo item
 *
 * Return: dynamically allocated JSON string, or NULL on failure
 */
char *todo_to_json(const todo_t *todo)
{
	char *json;
	int length;

	length = snprintf(NULL, 0,
			"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
			todo->id, todo->title, todo->description);
	json = malloc((size_t)length + 1);
	if (json == NULL)
		return (NULL);

	snprintf(json, (size_t)length + 1,
		"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
		todo->id, todo->title, todo->description);
	return (json);
}

/**
 * todos_to_json - serialize a list of todos to JSON
 * @list: array of todo pointers
 * @count: number of todos
 *
 * Return: dynamically allocated JSON string, or NULL on failure
 */
char *todos_to_json(todo_t **list, int count)
{
	char *json;
	char *item;
	char *updated;
	int index;
	int total;

	if (count == 0)
	{
		json = strdup("[]");
		return (json);
	}

	total = 2;
	json = malloc(2);
	if (json == NULL)
		return (NULL);
	strcpy(json, "[");

	for (index = 0; index < count; index++)
	{
		item = todo_to_json(list[index]);
		if (item == NULL)
		{
			free(json);
			return (NULL);
		}

		total += (int)strlen(item);
		if (index < count - 1)
			total += 1;

		updated = realloc(json, (size_t)total + 1);
		if (updated == NULL)
		{
			free(item);
			free(json);
			return (NULL);
		}
		json = updated;

		if (index > 0)
			strcat(json, ",");
		strcat(json, item);
		free(item);
	}

	updated = realloc(json, (size_t)total + 2);
	if (updated == NULL)
	{
		free(json);
		return (NULL);
	}
	json = updated;
	strcat(json, "]");
	return (json);
}
