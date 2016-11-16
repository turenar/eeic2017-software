#include <stdio.h>
#include <math.h>

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
		int sum = 0;
		for (int j = 0; j < NALPHA; j++) {
			sum += count[i][j];
		}

		double prob[NALPHA];
		for (int j = 0; j < NALPHA; j++) {
			prob[j] = count[i][j] / (double) sum;
		}

		double entropy = 0.;
		for (int j = 0; j < NALPHA; j++) {
			if (prob[j] != 0) {
				entropy -= prob[j] * (log2(prob[j]));
			}
		}

		printf("[%c]: ent = %e\n", 'a' + i, entropy);
	}
}

int is_lower_alpha(int c) {
	return 'a' <= c && c <= 'z';
}
