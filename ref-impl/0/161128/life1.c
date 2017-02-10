#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 50
#define WIDTH 70

int cell[HEIGHT][WIDTH];

void init_cells() {
	int i, j;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell[i][j] = 0;
		}
	}

	cell[20][30] = 1;
	cell[21][30] = 1;
	cell[22][31] = 1;
	cell[20][31] = 1;
	cell[20][32] = 1;
}

void print_cells(FILE* fp, int generation) {
	int i, j;
	int alive_count = 0;
	fprintf(fp, "----------\n");

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if ((cell[i][j] == 1)) {
				fputc('#', fp);
				alive_count++;
			} else {
				fputc(' ', fp);
			}
		}
		fputc('\n', fp);
	}
	fflush(fp);
	printf("generation = %d: alive = %d\n", generation, alive_count);
#ifdef _DEBUG
	usleep(100*1000);
#else
	sleep(1);
#endif
}

int count_adjacent_cells(int i, int j) {
	int n = 0;
	int k, l;
	for (k = i - 1; k <= i + 1; k++) {
		if (k < 0 || k >= HEIGHT) {
			continue;
		}
		for (l = j - 1; l <= j + 1; l++) {
			if (k == i && l == j) {
				continue;
			}
			if (l < 0 || l >= WIDTH) {
				continue;
			}
			n += cell[k][l];
		}
	}
	return n;
}

void update_cells() {
	int i, j;
	int cell_next[HEIGHT][WIDTH];

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			const int n = count_adjacent_cells(i, j);
			switch (n) {
				case 2:
					cell_next[i][j] = cell[i][j];
					break;
				case 3:
					cell_next[i][j] = 1;
					break;
				default:
					cell_next[i][j] = 0;
			}
		}
	}

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell[i][j] = cell_next[i][j];
		}
	}
}


int main() {
	int gen;
	FILE* fp;

	if ((fp = fopen("cells.txt", "a")) == NULL) {
		fprintf(stderr, "error: cannot open a file.\n");
		return 1;
	}

	init_cells();
	print_cells(fp, 0);

	for (gen = 1;; gen++) {
		update_cells();
		print_cells(fp, gen);
	}

	fclose(fp);

	return 0;
}
