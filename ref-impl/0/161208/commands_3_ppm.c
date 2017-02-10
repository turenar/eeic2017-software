#include "commands.h"
#include "util.h"
#include <stdio.h>

CommandReturn cmd_ppm(char* filename) {
	if (filename[0] == '\0') {
		fprintf(stderr, "usage: bmp <filename>\n");
		return COMMAND_ERROR;
	}

	FILE* fp = fopen(filename, "w");
	if (!fp) {
		perror(filename);
		return COMMAND_ERROR;
	}

	fprintf(fp, "P3\n%d %d\n255\n", width, height);
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			fprintf(fp, "%d %d %d\n", get_r(c), get_g(c), get_b(c));
		}
	}
	fclose(fp);
	printf("ppm file written: %s\n", filename);
	return COMMAND_OK;
}

