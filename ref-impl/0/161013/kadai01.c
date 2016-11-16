#include <stdio.h>

int should_be_nabeatu(int);

int main() {
	for (int i = 1; i <= 1000; ++i) {
		if (should_be_nabeatu(i)) {
			printf("%d!!!\n", i);
		} else {
			printf("%d\n", i);
		}
	}
}

int should_be_nabeatu(int i) {
	if (i % 3 == 0) {
		return 1;
	}
	while (i > 0) {
		if (i % 10 == 3) {
			return 1;
		}
		i = i / 10;
	}
	return 0;
}
