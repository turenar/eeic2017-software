#include "util.h"
#include <string.h>

char* tokenize(char* in, char** next) {
	char* p;
	for (p = in; *p != '\0'; ++p) {
		if (*p == ' ') {
			if (p == in) {
				in = p + 1;
			} else {
				*p = '\0';
				*next = p + 1;
				return in;
			}
		}
	}
	if (p != in) {
		*next = p; // '\0'
		return in;
	} else {
		return NULL;
	}
}

void remove_newline(char* str) {
	char* endptr = &str[strlen(str) - 1];
	if (*endptr == '\r' || *endptr == '\n') {
		*endptr = '\0'; // remove the line delimiter at the end
	}
}
