#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "painter.h"
#include "util.h"

#define BUFSIZE 1024

LinkedList* history;
uint32_t** canvas;

int height;
int width;
uint32_t fg_color = 0xffffffu;
uint32_t bg_color = 0x000000u;
int fill = 0;

const char* default_history_file = "history.txt";

void print_canvas(FILE* fp) {
	int x, y;

	fprintf(fp, "----------\n");

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			const char c = (char) (canvas[y][x] ? '#' : ' ');
			fputc(c, fp);
		}
		fputc('\n', fp);
	}
	fflush(fp);
}

void init_canvas(int w, int h) {
	canvas = (uint32_t**) malloc(sizeof(uint32_t*) * (size_t) h);
	for (int i = 0; i < h; ++i) {
		canvas[i] = (uint32_t*) calloc((size_t) w, sizeof(uint32_t));
	}
	width = w;
	height = h;
}

void set_canvas_color_safe(int x, int y, uint32_t color) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		canvas[y][x] = color;
	}
}

CommandReturn save_history(const char* filename) {
	if (filename == NULL) {
		filename = default_history_file;
	}

	FILE* fp;
	if ((fp = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "error: cannot open %s: %s\n", filename, strerror(errno));
		return COMMAND_ERROR;
	}

	LinkedListIterator it;
	for (list_iterator(history, &it); list_itr_has_next(&it);) {
		const char* s = list_itr_next(&it);
		fprintf(fp, "%s\n", s);
	}

	printf("saved as \"%s\"\n", filename);
	fclose(fp);
	return COMMAND_OK;
}

CommandReturn load_history(const char* filename) {
	if (filename == NULL) {
		filename = default_history_file;
	}

	FILE* fp;
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "error: cannot open %s: %s\n", filename, strerror(errno));
		return COMMAND_ERROR;
	}

	char buf[1024];
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		remove_newline(buf);
		interpret_command(buf);
		list_push_back(history, buf);
	}
	return COMMAND_OK;
}

// Interpret and execute a command
//   return value:
//     0, normal commands such as "line"
//     1, unknown or special commands (not recorded in history[])
//     2, quit
CommandReturn interpret_command(const char* command) {
	char buf[BUFSIZE];
	strcpy(buf, command);

	char* arg;
	const char* cname = tokenize(buf, &arg);
	if (cname == NULL) {
		return COMMAND_UNKNOWN;
	}

	for (CommandDecl* decl = command_decl; decl->name != NULL; ++decl) {
		if (strcmp(decl->name, cname) == 0) {
			return decl->fn(arg);
		}
	}

	fprintf(stderr, "error: unknown command: %s\n", cname);
	return COMMAND_UNKNOWN;
}
