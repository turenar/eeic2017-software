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

	double prob[NALPHA][NALPHA];
	for (int i = 0; i < NALPHA; i++) {
		int sum = 0;
		for (int j = 0; j < NALPHA; j++) {
			sum += count[i][j];
		}

		for (int j = 0; j < NALPHA; j++) {
			prob[i][j] = count[i][j] / (double) sum;
		}
	}

	double bd[NALPHA][NALPHA];
	for (int i = 0; i < NALPHA; ++i) {
		for (int j = 0; j < NALPHA; ++j) {
			double bc = 0.;
			for (int k = 0; k < NALPHA; ++k) {
				bc += sqrt(prob[i][k] * prob[j][k]);
			}
			if (bc == 0) {
				bd[i][j] = 100000;
			} else {
				bd[i][j] = -log(bc);
			}
		}
	}

	double max_bd = -1000000;
	double min_bd = 1000000;
	for (int i = 0; i < NALPHA; ++i) {
		for (int j = 0; j < NALPHA; ++j) {
			if (i != j) {
				if (bd[i][j] > max_bd) {
					max_bd = bd[i][j];
				}
				if (bd[i][j] < min_bd) {
					min_bd = bd[i][j];
				}
			}
		}
	}
	for (int i = 0; i < NALPHA; ++i) {
		for (int j = 0; j < NALPHA; ++j) {
			if (i < j && bd[i][j] == min_bd) {
				printf("[%c][%c]: min of bd = %e\n", 'a' + i, 'a' + j, bd[i][j]);
			}
		}
	}
	for (int i = 0; i < NALPHA; ++i) {
		for (int j = 0; j < NALPHA; ++j) {
			if (i < j && bd[i][j] == max_bd) {
				printf("[%c][%c]: max of bd = %e\n", 'a' + i, 'a' + j, bd[i][j]);
			}
		}
	}
}

int is_lower_alpha(int c) {
	return 'a' <= c && c <= 'z';
}
