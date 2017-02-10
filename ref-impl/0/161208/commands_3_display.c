#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

typedef enum {
	DISPLAY_2COLORS,
	DISPLAY_16COLORS,
	DISPLAY_256COLORS,
	DISPLAY_24BITS
} display_color_type;

static display_color_type display_type;

static CommandReturn show_2c();
static CommandReturn show_16c();
static int to_256c_rgb_unit(int);
static CommandReturn show_256c();
static CommandReturn show_24b();

CommandReturn cmd_show(char* args) {
	if (args[0] != '\0') {
		if (strcmp("2", args) == 0) {
			display_type = DISPLAY_2COLORS;
		} else if (strcmp("16", args) == 0) {
			display_type = DISPLAY_16COLORS;
		} else if (strcmp("256", args) == 0) {
			display_type = DISPLAY_256COLORS;
		} else if (strcmp("24b", args) == 0) {
			display_type = DISPLAY_24BITS;
		} else {
			fprintf(stderr, "unknown display type: %s\n", args);
			fprintf(stderr, "supported: 2 16 256 24b\n");
			return COMMAND_ERROR;
		}
	}

	switch (display_type) {
		case DISPLAY_2COLORS:
			return show_2c();
		case DISPLAY_16COLORS:
			return show_16c();
		case DISPLAY_256COLORS:
			return show_256c();
		case DISPLAY_24BITS:
			return show_24b();
		default:
			abort();
	}
}

static CommandReturn show_2c() {
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			if (*col_p++) {
				putchar('#');
			} else {
				putchar(' ');
			}
		}
		putchar('\n');
	}
	return COMMAND_SPECIAL;
}

static CommandReturn show_16c() {
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			int r = ((c & 0xff0000) >> 16) >= 128u;
			int g = (c & 0x00ff00) >> 8 >= 128u;
			int b = (c & 0x0000ff) >= 128u;
			int sp = r + (g << 1) + (b << 2);
			printf("\033[4%dm \033[0m", sp);
		}
		putchar('\n');
	}
	return COMMAND_SPECIAL;
}

static int to_256c_rgb_unit(int c) {
	// unit: 0   1   2   3   4   5
	// hex: 00  5f  87  af  d7  ff
	// dec: 00  95 135 175 215 255
	if (c < 55) {
		return 0;
	} else {
		return (c - 16) / 40;
	}
}

static CommandReturn show_256c() {
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			int r = (int) (c & 0xff0000) >> 16;
			int g = (int) (c & 0x00ff00) >> 8;
			int b = (int) (c & 0x0000ff);
			// 彩度
			int i_min = min3(r, g, b);
			int i_max = max3(r, g, b);
			int s = i_max == 0 ? 0 : ((i_max - i_min) * 256 / i_max);
			int sp;
			if (s < 32) {
				// quote from https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
				// grayscale from black to white in 24 steps
				sp = 232 + (i_max + i_min) * 24 / 510;
			} else {
				sp = 16 + 36 * to_256c_rgb_unit(r) + 6 * to_256c_rgb_unit(g) + to_256c_rgb_unit(b);
			}
			printf("\033[48;5;%dm \033[0m", sp);
		}
		putchar('\n');
	}
	return COMMAND_SPECIAL;
}

static CommandReturn show_24b() {
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			int r = (int) (c & 0xff0000) >> 16;
			int g = (int) (c & 0x00ff00) >> 8;
			int b = (int) (c & 0x0000ff);
			printf("\033[48;2;%d;%d;%dm \033[0m", r, g, b);
		}
		putchar('\n');
	}
	return COMMAND_SPECIAL;
}
