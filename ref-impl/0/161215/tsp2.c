#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

#define WIDTH 100
#define HEIGHT 50
#define MAX_CITIES 1000

typedef struct {
	int x;
	int y;
} City;

City city[MAX_CITIES];
char map[WIDTH * 2][HEIGHT];

int max(const int a, const int b) {
	return (a > b) ? a : b;
}

uint32_t xor128() {
	static uint32_t x = 123456789;
	static uint32_t y = 362436069;
	static uint32_t z = 521288629;
	static uint32_t w = 88675123;
	uint32_t t;

	t = x ^ (x << 11);
	x = y;
	y = z;
	z = w;
	return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

int next_rand_int(int ex_max) {
	return (int) (xor128() % (uint32_t) ex_max);
}

double next_rand_double() {
	return xor128() / (double) UINT32_MAX;
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
		draw_line(2 * city[c0].x, city[c0].y, 2 * city[c1].x, city[c1].y);
	}
}

void plot_cities(FILE* fp, const int n, const int* route) {
	fprintf(fp, "----------\n");

	memset(map, ' ', sizeof(map));

	for (int i = 0; i < n; i++) {
		char buf[100];
		sprintf(buf, "C_%d", i);
		for (int j = 0; j < (int) strlen(buf); j++) {
			const int x = 2 * city[i].x + j;
			const int y = city[i].y;
			map[x][y] = buf[j];
		}
	}

	draw_route(n, route);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < 2 * WIDTH; x++) {
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

void swap(int arr[], int i, int j) {
	int t = arr[i];
	arr[i] = arr[j];
	arr[j] = t;
}

void make_random_route(const int n, int route[]) {
	// フィッシャーイェーツのシャッフル
	for (int i = 0; i < n; ++i) {
		route[i] = i;
	}

	for (int i = n - 1; i > 1; --i) {
		int j = next_rand_int(i + 1);
		swap(route, i, j);
	}
}

double calc_total_distance(const int n, const int route[]) {
	// Compute the total distance
	double sum_d = 0;
	for (int i = 0; i < n; i++) {
		const int c0 = route[i];
		const int c1 = (i < n - 1) ? route[i + 1] : route[0];
		sum_d += distance(c0, c1);
	}
	return sum_d;
}

int next_candidate_swap(const int n, const int* prev, int* candidate) {
	int i, j;
	do {
		i = next_rand_int(n);
		j = next_rand_int(n);
	} while (i == j);

	// 手抜き
	memcpy(candidate, prev, sizeof(int) * (size_t) n);
	swap(candidate, i, j);
	return 0;
}

void insert(const int n, const int* prev, int* candidate, int from, int insert_pos) {
	int i = 0;
	int j = 0;
	for (; i < n; ++i) {
		if (i == from) {
			// skip
		} else if (i == insert_pos) {
			candidate[j++] = prev[from];
			candidate[j++] = prev[i];
		} else {
			candidate[j++] = prev[i];
		}
	}
}

int next_candidate_insert(const int n, const int* prev, int* candidate) {
	int from, insert_pos;
	do {
		from = next_rand_int(n);
		insert_pos = next_rand_int(n);
	} while (from == insert_pos);

	// 手抜き
	insert(n, prev, candidate, from, insert_pos);

	return 0;
}

int next_candidate_release_crossing(const int n, const int* prev, int* candidate) {
	// 交差を解消する
	for (int i = 1; i < n - 2; ++i) {
		for (int j = i + 1; j < n - 1; ++j) {
			City* a = &city[prev[i - 1]];
			City* b = &city[prev[i]];
			City* c = &city[prev[j]];
			City* d = &city[prev[j + 1]];

			// 交差判定 http://www.qiita.com/ykob/items/ab7f30c43a0ed52d16f2
			int ta = (c->x - d->x) * (a->y - c->y) + (c->y - d->y) * (c->x - a->x);
			int tb = (c->x - d->x) * (b->y - c->y) + (c->y - d->y) * (c->x - b->x);
			int tc = (a->x - b->x) * (c->y - a->y) + (a->y - b->y) * (a->x - c->x);
			int td = (a->x - b->x) * (d->y - a->y) + (a->y - b->y) * (a->x - d->x);

			if (tc * td < 0 && ta * tb < 0) {
				memcpy(candidate, prev, sizeof(int) * (size_t) n);
				// a b x y z c d を a c z y x b d にする
				int end = (j - i) / 2;
				for (int k = 0; k < end; ++k) {
					swap(candidate, i + k, j - k);
				}
				return 0;
			}
		}
	}
	return 1;
}

int next_candidate(const int n, const int* prev, int* candidate) {
	int x = next_rand_int(128);
	if (x < 8) {
		return next_candidate_release_crossing(n, prev, candidate);
	} else if (x < 32) {
		return next_candidate_swap(n, prev, candidate);
	} else {
		return next_candidate_insert(n, prev, candidate);
	}
}

double solve(FILE* fp, const int n, int route_out[]) {
	(void) fp;
	make_random_route(n, route_out);

	int* tmp_route = (int*) malloc(sizeof(int) * (size_t) n);
	int* prev = route_out; // 今のルートを元に
	int* candidate = tmp_route; // 次のルート候補を作る

	double min_d = calc_total_distance(n, route_out);
	double T = log2(n);
	const double r = 0.0005 / pow(log2(n), 2);
#ifdef _DEBUG
	int skipped = 0;
#endif
	while (T > 0.0001) {
		if (next_candidate(n, prev, candidate)) {
			continue;
		}

		double new_d = calc_total_distance(n, candidate);
		double rate = new_d < min_d ? 1 : exp(-(new_d - min_d) / T);
		if (rate >= next_rand_double()) {
			min_d = new_d;
#ifdef _DEBUG
			printf("\033[KT=%f, skipped=%d, score=%f\r", T, skipped, min_d); // not \n
			fflush(stdout);
			plot_cities(fp, n, candidate);

			usleep(100 * 1000);
			skipped = 0;
#else
			printf("\033[KT=%f, score=%f\r", T, min_d); // not \n
			fflush(stdout);
#endif
			// swap candidate, prev
			int* tmp = candidate;
			candidate = prev;
			prev = tmp;
		} else {
#ifdef _DEBUG
			skipped++;
#endif
		}
		T -= r * T;
	}
	if (prev != route_out) {
		memcpy(route_out, prev, sizeof(int) * (size_t) n);
	}
	free(tmp_route);
	return min_d;
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
	const double d = solve(fp, n, route);

	printf("total distance = %f\n", d);
	plot_cities(fp, n, route);

	return 0;
}
