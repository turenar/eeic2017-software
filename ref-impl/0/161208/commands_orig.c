#include "commands.h"
#include <string.h>
#include <stdlib.h>
#include "util.h"

CommandReturn cmd_line(char* args) {
	int x0, y0, x1, y1;
	char* endptr;
	x0 = (int) strtol(args, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	y0 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	x1 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	y1 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != '\0') {
		goto usage;
	}

	draw_line(x0, y0, x1, y1);
	return COMMAND_OK;

usage:
	fprintf(stderr, "usage: square <x0> <y0> <x1> <y1>\nall arguments must be int.\n");
	return COMMAND_ERROR;
}

CommandReturn cmd_save(char* arg) {
	CommandReturn result = save_history(*arg == '\0' ? NULL : arg);
	return result == COMMAND_OK ? COMMAND_SPECIAL : result;
}

CommandReturn cmd_undo(char* args) {
	UNUSED_ARG(args);
	list_pop_back(history);

	init_canvas(width, height);

	LinkedListIterator it;
	for (list_iterator(history, &it); list_itr_has_next(&it);) {
		const char* s = list_itr_next(&it);
		interpret_command(s);
	}
	return COMMAND_SPECIAL;
}

CommandReturn cmd_quit(char* args) {
	UNUSED_ARG(args);
	return COMMAND_EXIT;
}
