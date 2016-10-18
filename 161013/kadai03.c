#include <stdio.h>
#include <math.h>

void dot(double);

int main() {
	// ↓100 = 何行点を打つか
	for (int i = 0; i < 100; i++) {
		dot(sin(i * 0.3)); // ←0.3 = 大きいほど周期が短くなる
	}
}

void dot(double v) {
	int w = (int) (v * 10 + 10); // [-1 <= v <= 1]が[0 <= w <= 任意の正の整数]になるような式を入れる
	for (int i = 0; i < w; i++) {
		putchar(' ');
	}
	printf("*\n");
}
