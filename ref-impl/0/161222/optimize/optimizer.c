#include "optimizer.h"
#include <math.h>
#include <stdlib.h>
#include "logger.h"

static double calc_norm(unsigned int dim, double v[]);

int optimize_gradient_descent(const double alpha, optimizer_info* info) {
	double* g = (double*) malloc((size_t) info->dimension * sizeof(double));

	int iter = 0;
	while (++iter < DEFAULT_PATIENCE) {
		info->f_gradient(info, info->x, g);

		const double norm = calc_norm(info->dimension, g);
		if (isinf(norm) || isnan(norm)) {
			LOG(error, "norm is diverged. try smaller alpha");
			iter = -1;
			goto free_g;
		}
#if 0
#ifdef _DEBUG
		printf("%3d norm = %7.4f", iter, norm);
		for (unsigned int i = 0; i < info->dimension; i++) {
			printf(", x[%d] = %7.4f", i, info->x[i]);
		}
		printf("\n    f(x) = %7.4f", info->f_value(info, info->x));
		for (unsigned int i = 0; i < info->dimension; i++) {
			printf(", dx[%d] = %7.4f", i, g[i]);
		}
		putchar('\n');
#endif
#endif
		if (norm < 0.0001) {
			break;
		}

		for (unsigned int i = 0; i < info->dimension; i++) {
			info->x[i] -= alpha * g[i];
		}
	}

free_g:
	free(g);

	return iter;
}

void free_data_set(data_set* set) {
	while (set != NULL) {
		data_set* next = set->next;
		free(set);
		set = next;
	}
}

void free_optimizer(optimizer_info* info) {
	free_data_set(info->data);
	free(info->x);
	free((void*)info->labels[0]);
	free(info->labels);
}

static double calc_norm(const unsigned int dim, double v[]) {
	double tmp = 0;
	for (unsigned int i = 0; i < dim; i++) {
		tmp += v[i] * v[i];
	}
	tmp = sqrt(tmp);
	return tmp;
}

