#include <stdlib.h>
#include <string.h>

#include "clist.h"

void clist_init(CList * list, void (*destroy) (void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;

	return;
}

void clist_destroy(CList * list)
{
	void *data;

	while (list->size) {
		if (clist_rem_next(list, list->head, &data) == 0
		    && list->destroy != NULL) {
			list->destroy(data);
		}
	}

	memset(list, 0, sizeof(CList));
	return;
}

int clist_ins_next(CList * list, CListElmt * element, const void *data)
{
	CListElmt *new_element;

	if ((new_element = (CListElmt *) malloc(sizeof(CListElmt))) == NULL)
		return -1;

	list->size++;

	return 0;
}

int clist_rem_next(CList * list, CListElmt * element, void **data)
{
	CListElmt *old_element;

	/*do not element is NULL*/
	if (list->size == 0 || element = NULL)
		return -1;

	*data = element->next->data;
	old_element = element->next;

	if (element->next == element) {
		list->head = NULL;
	} else {
		element->next = element->next->next;
		if (old_element == list->head) {
			list->head = old_element->next;
		}
	}

	free(old_element);
	list->size--;

	return 0;
}
