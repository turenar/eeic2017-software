#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define HEIGHT 50
#define WIDTH 70
#define ALIVE_RATE 10
#define FIRST_READ_FILE "first_cells.txt"

int cell[HEIGHT][WIDTH];

void read_cells(FILE* fp) {
	for (int y = 0; y < HEIGHT; ++y) {
		char buf[4096];
		if (fgets(buf, sizeof(buf), fp) == NULL) {
			fprintf(stderr, "unexpected eof\n");
			exit(1);
		}
		for (int x = 0; x < WIDTH; ++x) {
			char c = buf[x];
			if (c == '#') {
				cell[y][x] = 1;
			} else if (c == ' ') {
				cell[y][x] = 0;
			} else { // '\n' '\0' etc...
				fprintf(stderr, "unexpected char\n");
				exit(1);
			}
		}
	}
}

void init_cells() {
	int i, j;
#ifdef _DEBUG
	srand((unsigned int) (time(NULL) + getpid()));
#else
	srand((unsigned int) time(NULL));
#endif

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell[i][j] = (rand() % ALIVE_RATE == 0);
		}
	}
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

	if ((fp = fopen(FIRST_READ_FILE, "r")) == NULL) {
		if (errno != ENOENT) {
			perror(FIRST_READ_FILE);
		}
		init_cells();
	} else {
		printf("file loaded\n");
		read_cells(fp);
		fclose(fp);
	}

	if ((fp = fopen("cells.txt", "a")) == NULL) {
		perror("cells.txt");
		return 1;
	}

	print_cells(fp, 0);

	for (gen = 1;; gen++) {
		update_cells();
		print_cells(fp, gen);
	}

	fclose(fp);

	return 0;
}
