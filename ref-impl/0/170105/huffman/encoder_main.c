#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "encoder.h"
#include "file_buffer.h"
#include "logger.h"
#include "utils.h"

static char* get_out_filename(const char*) __attribute_malloc__;


int main(int argc, char** argv) {
	FILE* rfp;
	FILE* wfp;
	if (argc == 1) {
		rfp = stdin;
		wfp = stdout;
		if (isatty(fileno(wfp))) {
			LOG(error, "compressed data not written to a terminal");
			return 1;
		}
	} else if (argc == 2) {
		rfp = fopen(argv[1], "rb");
		if (!rfp) {
			LOG(error, "%s: %s", argv[1], strerror(errno));
			return 1;
		}

		char* outfile = get_out_filename(argv[1]);
		if (file_exists(outfile)) {
			LOG(warn, "%s already exists", outfile);
			if (isatty(fileno(stdin))) {
				fprintf(stderr, "Overwrite? [y/N]> ");
				fflush(stderr);
				if (!get_yes_or_no_answer(false)) {
					LOG(info, "okay, exiting...");
					return 0;
				}
			} else {
				LOG(error, "You are not tty! Aborting...");
				return 1;
			}
		}
		wfp = fopen(outfile, "wb");
		if (!wfp) {
			LOG(error, "%s: %s", outfile, strerror(errno));
			free(outfile);
			return 1;
		}
		free(outfile);
	} else {
		LOG(error, "usage: %s [<filename>]", argv[0]);
		return 1;
	}

	FileBuffer* buffer = buf_new();
	buf_fully_read(buffer, rfp);
	if (buf_get_chunk_count(buffer) == 0) {
		LOG(error, "empty input!");
		return 1;
	}

	EncoderState* state = encoder_new(wfp);

	FileChunk* chunk;
	while (buf_next_chunk(buffer, &chunk)) {
		encoder_update_pass1(state, chunk->buf, chunk->len);
	}

	buf_rewind(buffer);
	fwrite("huf", 1, 4, wfp); // ファイルヘッダの書き込み: huf\0
	while (buf_next_chunk(buffer, &chunk)) {
		encoder_update_pass2(state, chunk->buf, chunk->len);
	}
	encoder_close(state);
	buf_free(buffer);

	return 0;
}

static char* get_out_filename(const char* filename) {
	size_t len = strlen(filename);
	char* ret = (char*) malloc(len + 4 + 1);
	memcpy(ret, filename, len);
	memcpy(ret + len, ".huf", 4 + 1);
	return ret;
}

