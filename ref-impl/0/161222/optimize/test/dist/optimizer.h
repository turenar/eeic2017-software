#pragma once

#include <stddef.h>

typedef struct data_set_tag {
	struct data_set_tag* next;
	double data[];
} data_set;

typedef struct optimizer_info_tag {
	const char** labels; // has data_dimension elements; labels and only [0] is allocated
	data_set* data; // CAUTION: data_set has constant size but different from sizeof(data_set_tag)
	unsigned int data_dimension;
	unsigned int data_row;
	unsigned int dimension;
	double* x; // has dimension elements

	double (* f_value)(struct optimizer_info_tag*, const double* x);
	void (* f_gradient)(struct optimizer_info_tag*, const double* x, double* g);
	void (* f_show_result)(struct optimizer_info_tag*);
} optimizer_info;

static const int DEFAULT_PATIENCE = 10000000;
int optimize_gradient_descent(const double alpha, optimizer_info* info);

void free_data_set(data_set*);
void free_optimizer(optimizer_info*);

static inline size_t sizeof_dataset(unsigned int dimension);

/* -- implementations -- */

static inline size_t sizeof_dataset(unsigned int dimension) {
	return sizeof(data_set) + sizeof(double) * dimension;
}
