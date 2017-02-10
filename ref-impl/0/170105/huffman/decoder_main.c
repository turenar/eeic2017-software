#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "decoder.h"
#include "logger.h"
#include "utils.h"

#define BUFSIZE 65536

static char* get_out_filename(const char*) __attribute_malloc__;
static bool check_header(FILE* fp);

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

	if (!check_header(rfp)) {
		return 1;
	}

	DecoderState* state = decoder_new(wfp);
	if (!state) {
		return 1;
	}

	int exitcode;
	unsigned char buf[BUFSIZE];

	while (true) {
		size_t len = fread(buf, 1, BUFSIZE, rfp);
		if (len == 0) {
			LOG(error, "data has partial huffman data?");
			exitcode = 1;
			goto free_decoder;
		} else if (decoder_update(state, buf, len)) {
			break;
		}
	}

	exitcode = 0;
free_decoder:
	decoder_close(state);

	return exitcode;
}

static char* get_out_filename(const char* filename) {
	size_t len = strlen(filename);
	if (len >= 5 && filename[len - 4] == '.' && filename[len - 3] == 'h'
	    && filename[len - 2] == 'u' && filename[len - 1] == 'f') {
		char* ret = (char*) malloc(len - 4 + 1);
		memcpy(ret, filename, len - 4);
		ret[len - 4] = '\0';
		return ret;
	} else {
		char* ret = (char*) malloc(len + 6 + 1);
		memcpy(ret, filename, len);
		memcpy(ret + len, ".unhuf", 6 + 1);
		return ret;
	}
}

static bool check_header(FILE* rfp) {
	char buf[4];
	size_t len = fread(buf, 1, 4, rfp);
	if (len < 4 || memcmp(buf, "huf", 4) != 0) {
		LOG(error, "not encoded data!");
		return false;
	}
	return true;
}
