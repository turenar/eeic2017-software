#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc <= 1) {
		for (unsigned int seed = 1; seed <= 1000; ++seed) {
			srand(seed);
			printf("=== seed: %d ===\n", seed);
			for (int i = 0; i < 10; ++i) {
				printf("%d\n", rand());
			}
		}
	} else {
		char* endptr;
		unsigned int seed = (unsigned int) strtoul(argv[1], &endptr, 10);
		if (*endptr != '\0') {
			fprintf(stderr, "invalid number: %s\n", argv[1]);
			return 1;
		}
		srand(seed);
		for (int i = 0; i < 10; ++i) {
			printf("%d\n", rand());
		}
	}
}
