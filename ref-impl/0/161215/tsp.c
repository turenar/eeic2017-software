#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

#define WIDTH 70
#define HEIGHT 40
#define MAX_CITIES 1000

typedef struct {
	int x;
	int y;
} City;

City city[MAX_CITIES];
char map[WIDTH][HEIGHT];

int max(const int a, const int b) {
	return (a > b) ? a : b;
}

void draw_line(const int x0, const int y0, const int x1, const int y1) {
	int i;
	const int n = max(abs(x1 - x0), abs(y1 - y0)) + 1;
	for (i = 0; i <= n; i++) {
		const int x = x0 + i * (x1 - x0) / n;
		const int y = y0 + i * (y1 - y0) / n;
		if (map[x][y] == ' ') {
			map[x][y] = '*';
		}
	}
}

void draw_route(const int n, const int* route) {
	if (route == NULL) {
		return;
	}

	int i;
	for (i = 0; i < n; i++) {
		const int c0 = route[i];
		const int c1 = (i < n - 1) ? route[i + 1] : route[0];
		draw_line(city[c0].x, city[c0].y, city[c1].x, city[c1].y);
	}
}

void plot_cities(FILE* fp, const int n, const int* route) {
	fprintf(fp, "----------\n");

	memset(map, ' ', sizeof(map));

	for (int i = 0; i < n; i++) {
		char buf[100];
		sprintf(buf, "C_%d", i);
		for (int j = 0; j < (int) strlen(buf); j++) {
			const int x = city[i].x + j;
			const int y = city[i].y;
			map[x][y] = buf[j];
		}
	}

	draw_route(n, route);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			const char c = map[x][y];
			fputc(c, fp);
		}
		fputc('\n', fp);
	}

	fflush(fp);
}

double distance(const int i, const int j) {
	const double dx = city[i].x - city[j].x;
	const double dy = city[i].y - city[j].y;
	return sqrt(dx * dx + dy * dy);
}

double solve1(const int ith, const int n, int route[], double sum_d) {
	if (ith == n) {
		return sum_d;
	}
	return route[0];
}

double solve(const int n, int route[]) {
	int i;

	route[0] = 0;  // Start from city[0]

	// This is dummy. Replace this with your own code to compute a solution.
	for (i = 1; i < n; i++) {
		route[i] = i;
	}

	// Compute the total distance
	double sum_d = 0;
	for (i = 0; i < n; i++) {
		const int c0 = route[i];
		const int c1 = (i < n - 1) ? route[i + 1] : route[0];
		sum_d += distance(c0, c1);
	}

	return sum_d;
}

int main(const int argc, const char** argv) {
	FILE* fp;
	const char* map_file = "map.txt";
	if ((fp = fopen(map_file, "a")) == NULL) {
		fprintf(stderr, "error: cannot open %s.\n", map_file);
		return 1;
	}

	if (argc != 2) {
		fprintf(stderr, "error: please specify the number of cities.\n");
		exit(1);
	}

	const int n = atoi(argv[1]);
	assert(n > 1 && n <= MAX_CITIES);

	int i;
	for (i = 0; i < n; i++) {
		city[i].x = rand() % (WIDTH - 5);
		city[i].y = rand() % HEIGHT;
	}

	plot_cities(fp, n, NULL);
	sleep(1);

	int route[MAX_CITIES];
	const double d = solve(n, route);

	printf("total distance = %f\n", d);
	plot_cities(fp, n, route);

	return 0;
}
