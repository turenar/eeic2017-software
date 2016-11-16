#include <stdio.h>

int strcmp1(const char* lhs, const char* rhs);

int main() {
	const char* a = "ABC";
	const char* b = "AAAA";

	printf("a: %s\n", a);
	printf("b: %s\n", b);
	printf("result: %d\n", strcmp1(a, b));
}

int strcmp1(const char* lhs, const char* rhs) {
	for (; *lhs != '\0' && *rhs != '\0'; lhs++, rhs++) {
		int d = *lhs - *rhs;
		if (d != 0) {
			return d;
		}
	}
	return *lhs - *rhs;
}
