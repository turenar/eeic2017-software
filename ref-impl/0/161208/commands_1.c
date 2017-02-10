#include "commands.h"
#include <stdlib.h>
#include <string.h>
#include "drawer.h"

CommandReturn cmd_rect(char* args) {
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

	draw_rect(x0, y0, x1, y1);
	return COMMAND_OK;

usage:
	fprintf(stderr, "usage: square <x0> <y0> <x1> <y1>\nall arguments must be int.\n");
	return COMMAND_ERROR;
}

CommandReturn cmd_circle(char* args) {
	int x0, y0, r;
	char* endptr;
	x0 = (int) strtol(args, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	y0 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	r = (int) strtol(endptr, &endptr, 10);
	if (*endptr != '\0') {
		goto usage;
	}

	draw_circle(x0, y0, r);
	return COMMAND_OK;

usage:
	fprintf(stderr, "usage: circle <x> <y> <r>\nall arguments must be int.\n");
	return COMMAND_ERROR;
}


