#include <stdio.h>

#define NALPHA 26

int main() {
	int count[NALPHA] = {0};

	int c;
	while ((c = getchar()) != EOF) {
		if (c >= 'a' && c <= 'z') {
			count[c - 'a']++;
		}
	}

	for (int i = 0; i < NALPHA; i++) {
		printf("%c : %d\n", 'a' + i, count[i]);
	}
}
