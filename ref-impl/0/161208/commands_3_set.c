#include "commands.h"
#include <string.h>
#include <stdlib.h>

static uint32_t parse_css_color(const char* color);
static uint32_t parse_hex_color(const char* color);
static CommandReturn set_color(const char* arg, uint32_t* to);

static uint32_t parse_css_color(const char* color) {
#define CHECK_CSS_COLOR(_hex_, _name_) do { if (strcmp(_name_, color) == 0) { return _hex_; } } while(0)
	CHECK_CSS_COLOR(0x000000, "black");
	CHECK_CSS_COLOR(0x808080, "gray");
	CHECK_CSS_COLOR(0xc0c0c0, "silver");
	CHECK_CSS_COLOR(0xffffff, "white");
	CHECK_CSS_COLOR(0x0000ff, "blue");
	CHECK_CSS_COLOR(0x000080, "navy");
	CHECK_CSS_COLOR(0x008080, "teal");
	CHECK_CSS_COLOR(0x008000, "green");
	CHECK_CSS_COLOR(0x00ff00, "lime");
	CHECK_CSS_COLOR(0x00ffff, "aqua");
	CHECK_CSS_COLOR(0xffff00, "yellow");
	CHECK_CSS_COLOR(0xff0000, "red");
	CHECK_CSS_COLOR(0xff00ff, "fuchsia");
	CHECK_CSS_COLOR(0x808000, "olive");
	CHECK_CSS_COLOR(0x800080, "purple");
	CHECK_CSS_COLOR(0x800000, "maroon");

	fprintf(stderr, "invalid css color name: %s\n", color);
	return INVALID_COLOR;
}


static uint32_t parse_hex_color(const char* color) {
	size_t len = strlen(color);
	char* endptr;
	uint32_t c = (uint32_t) strtoul(color, &endptr, 16);
	if (*endptr != '\0') {
		fprintf(stderr, "invalid hex: %s\n", color);
		return INVALID_COLOR;
	}
	if (len == 3) {
		uint32_t r = (c & 0xf00) >> 8;
		uint32_t g = (c & 0x0f0) >> 4;
		uint32_t b = (c & 0x00f);
		c = (r << 20) | (r << 16) | (g << 12) | (g << 8) | (b << 4) | b;
	} else if (len == 6) {
		// do nothing
	} else {
		fprintf(stderr, "invalid color hex (not 0xHEX nor 0xHEXHEX): %s\n", color);
		return INVALID_COLOR;
	}
	return c;
}

static CommandReturn set_color(const char* arg, uint32_t* to) {
	uint32_t c;
	if (arg[0] == '\0') {
		printf("#%06x\n", *to);
		return COMMAND_SPECIAL;
	} else if (arg[0] == '#') {
		c = parse_hex_color(arg + 1);
	} else if (arg[0] == '0' && arg[1] == 'x') {
		c = parse_hex_color(arg + 2);
	} else {
		c = parse_css_color(arg);
	}
	if (c == INVALID_COLOR) {
		return COMMAND_ERROR;
	} else {
		*to = c;
		return COMMAND_OK;
	}
}

CommandReturn cmd_fg(char* args) {
	return set_color(args, &fg_color);
}

CommandReturn cmd_bg(char* args) {
	return set_color(args, &bg_color);
}

CommandReturn cmd_fill(char* args) {
	if (args[0] == '\0') {
		printf("fill: %s\n", fill ? "yes" : "no");
		return COMMAND_SPECIAL;
	}
	if (strcmp(args, "y") == 0 || strcmp(args, "yes") == 0 || strcmp(args, "true") == 0) {
		fill = 1;
	} else if (strcmp(args, "n") == 0 || strcmp(args, "no") == 0 || strcmp(args, "false") == 0) {
		fill = 0;
	} else {
		printf("usage: fill <y|n>\n");
		return COMMAND_ERROR;
	}
	return COMMAND_OK;
}
