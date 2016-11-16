#include <stdio.h>
#include <math.h>

#define NALPHA 26

int main() {
	int count[NALPHA] = {0};

	int c;
	while ((c = getchar()) != EOF) {
		if (c >= 'a' && c <= 'z') {
			count[c - 'a']++;
		}
	}

	int sum = 0;
	for (int i = 0; i < NALPHA; i++) {
		sum += count[i];
	}

	double entropy = 0.;
	for (int i = 0; i < NALPHA; i++) {
		double prob = count[i] / (double) sum;
		entropy -= prob * (log2(prob));
	}
	printf("entropy = %e\n", entropy);
}
