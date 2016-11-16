#include <stdio.h>

#define NALPHA 26

int is_lower_alpha(int c);

int main() {
	int count[NALPHA][NALPHA];

	for (int i = 0; i < NALPHA; i++) {
		for (int j = 0; j < NALPHA; j++) {
			count[i][j] = 0;
		}
	}

	int c;
	int prev = ' ';
	while ((c = getchar()) != EOF) {
		if (is_lower_alpha(c) && is_lower_alpha(prev)) {
			count[prev - 'a'][c - 'a']++;
		}
		prev = c;
	}

	for (int i = 0; i < NALPHA; i++) {
		double prob[NALPHA];
		int sum = 0;
		for (int j = 0; j < NALPHA; j++) {
			sum += count[i][j];
		}
		for (int j = 0; j < NALPHA; j++) {
			prob[j] = count[i][j] / (double) sum;
		}

		double max = -1.;
		int maxidx;
		double min = 2.;
		int minidx;
		for (int j = 0; j < NALPHA; j++) {
			if (max < prob[j]) {
				maxidx = j;
				max = prob[j];
			}
			if (min > prob[j] && prob[j] > 0.) {
				minidx = j;
				min = prob[j];
			}
		}
		printf("[%c]: min = %c (%e), max = %c (%e)\n", 'a' + i, 'a' + minidx, prob[minidx], 'a' + maxidx, prob[maxidx]);
	}
}

int is_lower_alpha(int c) {
	return 'a' <= c && c <= 'z';
}
