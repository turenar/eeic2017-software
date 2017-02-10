#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_ITEMS 1000

#define LARGE_COUNT 6
const double max_weights = 20;      // capacity of the knapsack
double values[MAX_ITEMS + 1]; // value  (v[0] is not used)
double weights[MAX_ITEMS + 1]; // weight (w[0] is not used)
int params[MAX_ITEMS + 1];    // in or out (x[0] is not used)
double best_value = 0;
int best_params[MAX_ITEMS + 1];

void print_solution(const int count, const int par[MAX_ITEMS + 1]) {
	int i;
	double sum_value = 0;
	double sum_weight = 0;
	for (i = 1; i <= count; i++) {
		printf("%d", par[i]);
		if (par[i] == 0) {
			continue;
		}
		sum_value += values[i];
		sum_weight += weights[i];
	}
	printf(", total_value = %5.1f, total_weight = %5.1f\n", sum_value, sum_weight);
}

double search(const int number, const int count, const double sum_v, const double sum_w) {
	assert(number >= 1 && sum_v >= 0 && sum_w >= 0);
	if (sum_w > max_weights) {
		return -1;
	}

	if (number == count + 1) {
		if (count <= LARGE_COUNT) {
			print_solution(count, params);
		}
		if (sum_v > best_value) {
			best_value = sum_v;
			for (int j = 1; j <= count; j++) {
				best_params[j] = params[j];
			}
		}
		return sum_v;
	}

	params[number] = 0;
	const double v0 = search(number + 1, count, sum_v, sum_w);

	params[number] = 1;
	const double v1 = search(number + 1, count, sum_v + values[number], sum_w + weights[number]);

	return (v0 > v1) ? v0 : v1;
}

double solve(const int n) {
	const double value = search(1, n, 0, 0);

	return value;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "error: please specify the number of items.\n");
		exit(1);
	}

	const int n = atoi(argv[1]);
	assert(n <= MAX_ITEMS);

	printf("max_weights = %.1f\n", max_weights);

	// set values and weights randomly
	for (int i = 1; i <= n; i++) {
		values[i] = 0.1 * (rand() % 200);
		weights[i] = 0.1 * (rand() % 200);
		params[i] = 0;
		printf("values[%d] = %4.1f, weights[%d] = %4.1f\n", i, values[i], i, weights[i]);
	}
	printf("-----\n");

	solve(n);

	printf("-----\nbest solution:\n");
	print_solution(n, best_params);
	return 0;
}
