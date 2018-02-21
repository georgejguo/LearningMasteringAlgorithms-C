#include <stdlib.h>
#include <string.h>
#include "list.h"

void list_init(List * list, void (*destroy) (void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}

void list_destroy(List * list)
{
	void *data;

	while (list->size > 0) {
		if (list_rem_next(list, NULL, (void **)&data) == 0
		    && list->destroy != NULL) {
			list->destroy(data);
		}
	}

	return;
}

int list_ins_next(List * list, ListElmt * element, const void *data)
{
	ListElmt *new_element;

	if ((new_element = (ListElmt *) malloc(sizeof(ListElmt))) == NULL)
		return -1;

	new_element->data = (void *)data;

	if (element == NULL) {
		if (list->head == NULL)
			list->tail = new_element;

		new_element->next = list->head;
		list->head = new_element;
	} else {
		new_element->next = element->next;
		element->next = new_element;

		if (element->next == NULL)
			list->tail = element;
	}

	list->size++;

	return 0;
}

int list_rem_next(List * list, ListElmt * element, void **data)
{
	ListElmt *old_element;

	if (list->size == 0)
		return -1;

	if (element == NULL) {
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;

		if (list->head == NULL)
			list->tail = NULL;

	} else {
		if (element->next == NULL)
			return -1;

		*data = element->next->next;
		old_element = element->next;
		element->next = element->next->next;

		if (element->next = NULL)
			list->tail = element;
	}

	free(old_element);

	list->size--;

	return 0;
}
