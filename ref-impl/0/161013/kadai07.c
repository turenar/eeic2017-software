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
		for (int j = 0; j < NALPHA; j++) {
			printf("[%c][%c] : %d\n", 'a' + i, 'a' + j, count[i][j]);
		}
	}
}

int is_lower_alpha(int c) {
	return 'a' <= c && c <= 'z';
}
