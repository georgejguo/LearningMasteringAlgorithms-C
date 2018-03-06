#include "clist.h"
#include "page.h"

int replace_page(CListElmt ** current)
{
	while ((*current)->data->reference) {
		(*current)->data->reference = 0;
		*current = clist_next(*current);
	}

	return (*current)->data->number;
}
