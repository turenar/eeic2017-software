#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "painter.h"
#include "util.h"

#ifndef DEFAULT_WIDTH
#define DEFAULT_WIDTH 70
#endif
#ifndef DEFAULT_HEIGHT
#define DEFAULT_HEIGHT 40
#endif

#define BUFSIZE 1024

static int batch_main(const char* filename);
static int interactive_loop();

static int batch_main(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		perror(filename);
		return 1;
	}

	history = list_new();
	init_canvas(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	char buf[BUFSIZE];
	int has_error = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		remove_newline(buf);
		const int result = interpret_command(buf);
		switch (result) {
			case COMMAND_EXIT:
				goto exit_label;
			case COMMAND_OK:
			case COMMAND_SPECIAL:
				list_push_back(history, buf);
				break;
			default:
				has_error = 1;
		}
	}
exit_label:
	return has_error;
}

static int interactive_loop() {

	const char* filename = "canvas.txt";
	FILE* fp;
	char buf[BUFSIZE];

	if ((fp = fopen(filename, "a")) == NULL) {
		fprintf(stderr, "error: cannot open %s.\n", filename);
		return 1;
	}

	history = list_new();
	init_canvas(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	print_canvas(fp);
	do {
		printf("%u> ", (unsigned int) list_get_count(history));
		if (fgets(buf, BUFSIZE, stdin) == NULL) {
			printf("\nexit ok? [y/N]> ");
			if (fgets(buf, BUFSIZE, stdin) == NULL) {
				break;
			}
			remove_newline(buf);
			if (strcmp(buf, "y") == 0 || strcmp(buf, "yes") == 0) {
				break;
			} else if (*buf == '\0' || strcmp(buf, "n") == 0 || strcmp(buf, "no") == 0) {
				// valid answer
			} else {
				fprintf(stderr, "answer with y or n!\n");
			}
			continue;
		}

		remove_newline(buf);
		const int r = interpret_command(buf);
		if (r == COMMAND_EXIT) {
			break;
		}
		if (r == COMMAND_OK) {
			list_push_back(history, buf);
		}
		print_canvas(fp);
	} while (1);

	fclose(fp);
	return 0;
}

int main(int argc, char** argv) {
	if (argc >= 2) {
		return batch_main(argv[1]);
	} else {
		return interactive_loop();
	}
}
