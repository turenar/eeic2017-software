#include "commands.h"
#include "util.h"
#include <stdlib.h>

CommandReturn cmd_resize(char* args) {
	// w h
	char* endptr;
	int w, h;
	w = (int) strtol(args, &endptr, 10);
	if (*endptr != ' ') {
		goto print_usage;
	}
	h = (int) strtol(endptr, &endptr, 10);
	if (*endptr != '\0') {
		goto print_usage;
	}
	goto parse_success;

print_usage:
	fprintf(stderr, "usage: resize <width> <height> [<x0> <y0>]\n");
	return COMMAND_ERROR;

parse_success:
	(void) 0; // error: ラベルは文または宣言の一部にのみなることができます
	uint32_t** old_canvas = canvas;
	int old_w = width;
	int old_h = height;

	init_canvas(w, h);

	int max_w, max_h;
	max_w = min(w, old_w);
	max_h = min(h, old_h);

	uint32_t** out_row_p = canvas;
	uint32_t** in_row_p = old_canvas;
	for (int y = 0; y < max_h; ++y) {
		uint32_t* out_col_p = *out_row_p++;
		uint32_t* in_col_p = *in_row_p++;
		for (int x = 0; x < max_w; ++x) {
			*out_col_p++ = *in_col_p++;
		}
	}

	uint32_t** row_p = old_canvas;
	for (int y = 0; y < old_h; ++y) {
		free(*row_p++);
	}
	free(old_canvas);

	return COMMAND_OK;
}
