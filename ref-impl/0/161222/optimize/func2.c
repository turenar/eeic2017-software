#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* loc;   // location name
	double alt;  // altitude (km)
	double temp; // temperature (centigrade)
} Sample;

typedef void (* func_gradient_t)(const double[], double[]);
typedef double (* func_value_t)(const double[]);

const Sample data[] = {
		{"Nozawa-onsen",   0.576,  22.3},
		{"Iiyama",         0.313,  23.3},
		{"Nagano",         0.4182, 23.8},
		{"Oomachi",        0.784,  21.1},
		{"Sugadaira",      1.253,  18.5},
		{"Karuizawa",      0.9991, 19.5},
		{"Matsumoto",      0.610,  23.6},
		{"Nagawa",         1.068,  19.7},
		{"Suwa",           0.760,  22.7},
		{"Nobeyama",       1.350,  18.4},
		{"Ina",            0.633,  22.7},
		{"Nagiso",         0.560,  22.3},
		{"Iida",           0.5164, 23.9},
		{"Minami-shinano", 0.407,  23.5},
		{NULL,             0,      0}
};

int f_dimension() {
	return 2;
}

double f_value(const double x[]) {
	const double a = x[0];
	const double b = x[1];
	double sum = 0;
	for (const Sample* p = data; p->loc != NULL; ++p) {
		double tmp = p->temp - (a * p->alt + b);
		sum += tmp * tmp;
	}
	return sum;
}

void f_gradient(const double x[], double g[]) {
	const double a = x[0];
	const double b = x[1];

	double dEda = 0;
	double dEdb = 0;
	for (const Sample* p = data; p->loc != NULL; ++p) {
		dEda += -2 * (p->temp - a * p->alt - b) * p->alt;
		dEdb += -2 * (p->temp - a * p->alt - b);
	}
	g[0] = dEda;
	g[1] = dEdb;
}

static double calc_norm(const int dim, double v[]) {
	double tmp = 0;
	int i = 0;
	for (i = 0; i < dim; i++) {
		tmp += v[i] * v[i];
	}
	tmp = sqrt(tmp);
	return tmp;
}

int optimize_gradient_descent(const double alpha, const int dim, double* x, func_gradient_t calc_grad,
                              func_value_t f_value) {
	double* g = malloc((size_t) dim * sizeof(double));

	int iter = 0;
	while (++iter < 10000) {

		(*calc_grad)(x, g);

		const double norm = calc_norm(dim, g);
		printf("%3d norm = %7.4f", iter, norm);
		for (int i = 0; i < dim; i++) {
			printf(", x[%d] = %7.4f", i, x[i]);
		}
		printf(", f(x) = %7.4f", f_value(x));
		putchar('\n');

		if (norm < 0.001) {
			break;
		}

		for (int i = 0; i < dim; i++) {
			x[i] -= alpha * g[i];
		}
	}

	free(g);

	return iter;
}

int main(const int argc, const char** argv) {
	const double alpha = (argc == 2) ? atof(argv[1]) : 0.01;

	int i;
	const int dim = f_dimension();

	double* x = malloc((size_t) dim * sizeof(double));
	for (i = 0; i < dim; i++) {
		x[i] = 0;
	}

	printf("alpha = %f\n", alpha);

	const int iter = optimize_gradient_descent(alpha, dim, x, f_gradient, f_value);

	printf("number of iterations = %d\n", iter);

	free(x);

	return 0;
}


