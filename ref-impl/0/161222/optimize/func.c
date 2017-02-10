#include "func.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "optimizer.h"

static double f_rl_value(struct optimizer_info_tag*, const double* x);
static void f_rl_gradient(struct optimizer_info_tag*, const double* x, double* g);
static void f_rl_show_result(struct optimizer_info_tag*);

bool regress_linear(struct optimizer_info_tag* info) {
	if (info->data_dimension <= 1) {
		LOG(error, "no describe variables");
		return false;
	}

	info->dimension = info->data_dimension;
	info->x = (double*) calloc(info->dimension, sizeof(double));
	info->f_gradient = f_rl_gradient;
	info->f_value = f_rl_value;
	info->f_show_result = f_rl_show_result;
	return true;
}

static double f_rl_value(struct optimizer_info_tag* info, const double* x) {
	double square_error_sum = 0;
	data_set* set = info->data;
	while (set != NULL) {
		// x[0] == const
		double error = set->data[0] - x[0];
		for (unsigned int i = 1; i < info->data_dimension; ++i) {
			error -= x[i] * set->data[i];
		}
		square_error_sum += error * error;
		set = set->next;
	}
	return square_error_sum;
}

static void f_rl_gradient(struct optimizer_info_tag* info, const double* x, double* g) {
	data_set* set = info->data;
	memset(g, 0, sizeof(double) * info->dimension);
	while (set != NULL) {
		double common = set->data[0] - x[0];
		for (unsigned int i = 1; i < info->data_dimension; ++i) {
			common -= set->data[i] * x[i];
		}
		g[0] += -2 * common;
		for (unsigned int i = 1; i < info->data_dimension; ++i) {
			g[i] += -2 * common * set->data[i];
		}
		set = set->next;
	}
}

static void f_rl_show_result(struct optimizer_info_tag* info) {
	printf("%s = %f", info->labels[0], info->x[0]);
	for (unsigned int i = 1; i < info->data_dimension; ++i) {
		printf(" + %f * %s", info->x[i], info->labels[i]);
	}
	printf("\n");

	printf("rmse: %f\n", sqrt(info->f_value(info, info->x) / info->data_row));
}
