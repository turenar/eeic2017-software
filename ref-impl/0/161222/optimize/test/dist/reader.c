#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

static char* tokenize(char*, char** next, char delimiter);
static char* my_strdup(const char*);
static size_t estimate_required_header_len(unsigned int dimension);

bool read_csv(const char* filename, char col_delimiter, bool has_header, optimizer_info* info) {
	FILE* fp;
	if (filename == NULL) {
		fp = stdin;
	} else {
		fp = fopen(filename, "r");
		if (fp == NULL) {
			perror(filename);
			return false;
		}
	}

	char buf[65536];
	unsigned int col;
	if (fgets(buf, sizeof(buf), fp) != NULL) {
		col = 1;
		for (char* p = buf; *p != '\0'; ++p) {
			if (*p == col_delimiter) {
				++col;
			}
		}
	} else {
		LOG(error, "empty input");
		return false;
	}

	const char** header_arr = (const char**) malloc(sizeof(char*) * col);
	const char** header_p = header_arr;
	if (has_header) {
		char* header_str = my_strdup(buf);
		char* next_header_part;
		while ((next_header_part = tokenize(header_str, &header_str, col_delimiter)) != NULL) {
			*header_p++ = next_header_part;
		}
		if (fgets(buf, sizeof(buf), fp) == NULL) {
			LOG(error, "input has only header");
			goto free_header;
		}
	} else {
		char* header_str = (char*) malloc(estimate_required_header_len(col));
		for (unsigned int i = 0; i < col; ++i) {
			int written_len = sprintf(header_str, "x[%u]", i);
			*header_p++ = header_str;
			header_str += written_len + 1;
		}
	}

	LOG(debug, "input columns: %d", col);
	data_set* first = NULL;
	data_set* last;
	unsigned int reading_row = 0;
	do {
		++reading_row;
		char* str_p = buf;
		data_set* next = (data_set*) malloc(sizeof_dataset(col));
		double* val_p = next->data;
		for (unsigned int reading_col = 0; reading_col < col; ++reading_col) {
			char* start = tokenize(str_p, &str_p, col_delimiter);
			if (start == NULL) {
				LOG(error, "unexpected eof");
				free(next);
				goto free_data_set;
			}
			char* endptr;
			double val = strtod(start, &endptr);
			if (*endptr != '\0') {
				LOG(error, "parse error [line %u column %d]", reading_row, (int) (endptr - buf + 1));
				free(next);
				goto free_data_set;
			}
			*val_p++ = val;
		}
		if (*str_p != '\0' && *str_p != '\r' && *str_p != '\n') {
			LOG(error, "column count mismatch [line %u column %d]", reading_row, (int) (str_p - buf + 1));
			free(next);
			goto free_data_set;
		}

		if (first == NULL) {
			first = next;
			last = next;
		} else {
			last->next = next;
			last = next;
		}
	} while (fgets(buf, sizeof(buf), fp) != NULL);
	last->next = NULL;
	info->data = first;
	info->data_dimension = col;
	info->data_row = reading_row;
	info->labels = header_arr;

	LOG(debug, "input rows: %u", reading_row);
	return true;

free_data_set:
	while (first != NULL) {
		data_set* next = first;
		free(first);
		first = next;
	}
free_header:
	free((void*) header_arr[0]);
	free(header_arr);
	return false;
}

static char* tokenize(char* str, char** next, char delimiter) {
	if (*str == '\0') {
		return NULL;
	}
	char* del_p;
	for (del_p = str; *del_p != '\0'; ++del_p) {
		char c = *del_p;
		if (c == '\r' || c == '\n' || c == delimiter) {
			*del_p = '\0';
			*next = del_p + 1;
			return str;
		}
	}
	*next = del_p; // &('\0')
	return str;
}

static char* my_strdup(const char* in) {
	char* out = (char*) malloc(strlen(in) + 1);
	strcpy(out, in);
	return out;
}

static size_t estimate_required_header_len(unsigned int dimension) {
	size_t required_len_per_label = sizeof("x[0]");
	unsigned int order = 0;
	for (unsigned int max_ex_num = 10; dimension >= max_ex_num; ++order) {
		max_ex_num *= 10;
	}
	return (required_len_per_label + order) * dimension; // ちょっと無駄があるが無視して良いはず
}
