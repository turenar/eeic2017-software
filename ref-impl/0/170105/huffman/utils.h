#pragma once

#include <stdbool.h>
#include <stddef.h>


static inline size_t max(size_t a, size_t b);
static inline size_t min(size_t a, size_t b);

bool file_exists(const char* filename);
bool get_yes_or_no_answer(bool default_answer);

/* -- implementations -- */

static inline size_t max(size_t a, size_t b) {
	return (a > b) ? a : b;
}

static inline size_t min(size_t a, size_t b) {
	return (a < b) ? a : b;
}
