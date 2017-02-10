#include "commands.h"
#include "util.h"
#include <stdlib.h>


struct pair {
	double x;
	double y;
};

static int compare_coord(const void* a, const void* b);

static int compare_coord(const void* va, const void* vb) {
	struct pair* a = (struct pair*) va;
	struct pair* b = (struct pair*) vb;
	return (int) (b->y - a->y);
}

CommandReturn cmd_triangle(char* args) {
	struct pair p[3];

	char* endptr = args;
	for (int i = 0; i < 3; ++i) {
		p[i].x = (int) strtol(endptr, &endptr, 10);
		if (*endptr != ' ') {
			goto print_usage;
		}
		p[i].y = (int) strtol(endptr, &endptr, 10);
		if (i == 2 && *endptr != '\0') {
			goto print_usage;
		} else if (i != 2 && *endptr != ' ') {
			goto print_usage;
		}
	}

	goto parse_ok;

print_usage:
	fprintf(stderr, "usage: triangle <x0> <y0> <x1> <y1> <x2> <y2>\n");
	return COMMAND_ERROR;

parse_ok:
	qsort(p, 3, sizeof(struct pair), compare_coord);

	double r0 = (p[0].y == p[1].y) ? 0 : ((p[0].x - p[1].x) / (p[0].y - p[1].y));
	double r1 = (p[0].y == p[2].y) ? 0 : ((p[0].x - p[2].x) / (p[0].y - p[2].y));
	double r2 = (p[1].y == p[2].y) ? 0 : ((p[1].x - p[2].x) / (p[1].y - p[2].y));

	double y = p[0].y;
	for (; y<p[1].y; ++y) {
	}

	return COMMAND_OK;
}
