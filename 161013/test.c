#include <stdio.h>
#include <math.h>

#define NALPHA 26

int main() {

	int count[NALPHA][NALPHA];
	double prob[NALPHA][NALPHA];
	int c, i, j, sum;
	int first, second;


	for (i = 0; i < NALPHA; i++) {
		for (j = 0; j < NALPHA; j++) {
			count[i][j] = 0;
		}
	}
	second = ' ';

	while ((c = getchar()) != EOF) {
		first = second;
		second = c;
		for (i = 0; i < NALPHA; i++) {
			for (j = 0; j < NALPHA; j++) {
				if (first == 'a' + i && second == 'a' + j) {
					count[first - 'a'][second - 'a']++;
				}
			}
		}
	}

	for (i = 0; i < NALPHA; i++) {
		sum = 0;

		for (j = 0; j < NALPHA; j++) {
			sum = sum + count[i][j];
		}
		for (j = 0; j < NALPHA; j++) {
			prob[i][j] = (double) count[i][j] / (double) sum;
		}
	}


	//prob[i][j] = (double)count[i][j]/(double)sum;
	for (i = 0; i < NALPHA; i++) {
		double entropy = 0;

		for (j = 0; j < NALPHA; j++) {
			if (prob[i][j] != 0) {
				entropy += prob[i][j] * (-log2(prob[i][j]));
			} else {
				entropy += 0;
			}
		}
		printf("[%c]: ent = %f\n", 'a' + i, entropy);
	}
	return 0;

}
