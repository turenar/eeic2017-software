#include <stdio.h>
#include <math.h>

void dot(double);

int main() {
	for (int i = 0; i < 100; i++) {
		dot(sin(i * 0.3));
	}
}

void dot(double v) {
	int w = (int) (v * 10 + 12);
	for (int i = 0; i < w; i++) {
		putchar(' ');
	}
	printf("*\n");
}
