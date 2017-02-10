#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static bool str_equals(const char*, const char*);
static void drop_newline(char*);

bool file_exists(const char* filename) {
	struct stat st;
	return stat(filename, &st) == 0;
}

bool get_yes_or_no_answer(bool default_answer) {
	char buf[65536];
	while (fgets(buf, sizeof(buf), stdin)) {
		drop_newline(buf);
		if (buf[0] == '\0') {
			break;
		} else if (str_equals(buf, "y") || str_equals(buf, "yes")) {
			return true;
		} else if (str_equals(buf, "n") || str_equals(buf, "no")) {
			return false;
		} else {
			fprintf(stderr, "Answer yes or no> ");
			fflush(stderr);
		}
	}
	return default_answer;
}

static bool str_equals(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}

static void drop_newline(char* str) {
	size_t len = strlen(str);
	if (len >= 2 && str[len - 2] == '\r' && str[len - 1] == '\n') {
		str[len - 2] = '\0';
	} else if (len >= 1 && str[len - 1] == '\n') {
		str[len - 1] = '\0';
	}
}
