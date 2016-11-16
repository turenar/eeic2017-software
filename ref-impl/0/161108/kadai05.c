#include <stdio.h>
#include <math.h>

double newton(double x);

//double f(double x) { return x - 1 - log(x + 1); }
//double f(double x) { return x * exp(x) + log(x + 1); }
double f(double x) {
	return x * x * x * x - 2 * x * x * x - 13 * x * x + 14 * x + 24;
} // (x+1)(x-2)(x+3)(x-4)=(xx-x-2)(xx-x-12)=(A-2)(A-12)=AA-14A+24=(xx-x)^2 - 14(xx-x) + 24

//double df(double x) { return 1 - 1. / (x + 1); }
//double df(double x) { return (x + 1) * exp(x) + 1. / (x + 1); }
double df(double x) { return 4 * x * x * x - 6 * x * x - 26 * x + 14; }

int main() {
	double initial_value = -0.95;

	printf("x1 = %f\n", newton(initial_value));

	initial_value = 3.0;
	printf("x2 = %f\n", newton(initial_value));
}

double newton(double val) {
	double x1 = val - f(val) / df(val);

	if (fabs(x1 - val) < 0.0001) {
		return x1;
	} else {
		return newton(x1);
	}
}
