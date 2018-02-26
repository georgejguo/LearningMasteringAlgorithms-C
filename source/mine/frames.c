#include <stdio.h>
#include "frames.h"
#include "list.h"

int alloc_frame(List * frames)
{
	int frame_number, *data;

	if (list_size(frames) == 0) {
		return -1;
	} else {
		if (list_rem_next(frames, NULL, (void **)&data) == 0) {
			frame_number = *data;
			free(data);
		} else {
			return -1;
		}
	}

	return frame_number;
}

int free_frame(List * frames, int frame_number)
{
	int *data;

	if ((data = (int *)malloc(sizeof(int))) == NULL) {
		return -1;
	}

	*data = frame_number; // data = &frame_number;  X

	if (list_ins_next(frames, NULL, (void *)data) != 0) {
		return -1;
	}

	return 0;
}
