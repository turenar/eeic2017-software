#include <stdio.h>
#include <stdlib.h>

double pie(double x0, int N);

/*
 * pie(0.100000, 1) = 0.200000
 * pie(0.100000, 2) = 0.400000
 * pie(0.100000, 3) = 0.800000
 * pie(0.100000, 4) = 0.400000 ←しばらく振動する
 * pie(0.100000, 5) = 0.800000
 * pie(0.100000, 6) = 0.400000
 * pie(0.100000, 7) = 0.800000
 * pie(0.100000, 8) = 0.400000
 * pie(0.100000, 9) = 0.800000
 * pie(0.100000, 10) = 0.400000
 *
 * pie(0.100000, 36) = 0.400000
 * pie(0.100000, 37) = 0.799999 ←誤差が蓄積され%f出力に現れ始める
 * pie(0.100000, 38) = 0.400002
 *
 * pie(0.100000, 49) = 0.796875
 * pie(0.100000, 50) = 0.406250 ←1/2の和の形に落ち着きはじめる
 *
 * pie(0.100000, 55) = 1.000000
 * pie(0.100000, 56) = 0.000000 ←0
 *
 * pie(0.100000, 100) = 0.000000
 * pie(0.100000, 1000) = 0.000000
 */
int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "usage: %s <x0> <N>\n", argv[0]);
		return 1;
	}

	char* endptr;
	double x0 = strtod(argv[1], &endptr);
	if (*endptr != '\0') {
		fprintf(stderr, "invalid number: %s\n", argv[1]);
		return 1;
	}
	int N = (int) strtol(argv[2], &endptr, 10);
	if (*endptr != '\0') {
		fprintf(stderr, "invalid number: %s\n", argv[2]);
		return 1;
	}

	printf("pie(%f, %d) = %f\n", x0, N, pie(x0, N));
	return 0;
}

double pie(double x0, int N) {
	double x = x0;
	for (int i = 0; i < N; ++i) {
		if (x <= 0.5) {
			x = 2 * x;
		} else {
			x = 2 - 2 * x;
		}
	}
	return x;
}
