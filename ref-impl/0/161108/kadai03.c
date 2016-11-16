#include <stdio.h>
#include <stdlib.h>

#define MAXCT 300000

double random_float(double, double);

int main() {
	unsigned int seed;

	printf("enter seed: ");
	scanf("%u", &seed);
	srand(seed);

	int ct = 0;
	for (int i = 0; i < MAXCT; ++i) {
		double x = random_float(-1, 1);
		double y = random_float(-1, 1);

		if (x * x + y * y <= 1) {
			ct++;
		}
	}

	printf("Approx. PI = %f\n", 4.0 * ct / MAXCT);
	return 0;
}

double random_float(double min, double max) {
	return min + ((max - min) * rand()) / RAND_MAX;
}
