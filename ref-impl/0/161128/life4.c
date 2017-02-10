#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define HEIGHT 50
#define WIDTH 70

/*
 * セルの概念を羊 (増殖者) とライオン (捕食者) に分割し、
 *  * 羊は周囲2〜4匹で生存、3〜4で繁殖
 *  * ライオンは周辺に羊がいるならば、ランダムに一匹を選び食い殺しながらそのマスに移動する。
 *      この時近くに他のライオンがいれば元のマスに子供を産む。
 *    羊が居なければ餓死する。
 * というルールのライフゲームである。
 *
 * 初期配置に依るが、100〜2000時間後には「周囲に」羊が居なくなることにより、ライオンのみが死に絶えることが多い。
 * 稀にすべての羊が殺されることでどちらも絶滅することがある。
 */

enum animal_type {
	NONE, SHEEP, LION
};

enum animal_type cell[HEIGHT][WIDTH];

void init_cells() {
	int i, j;
#ifdef _DEBUG
	srand((unsigned int) (time(NULL) + getpid()));
#else
	srand((unsigned int) time(NULL));
#endif

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			int r = rand() % 100;
			enum animal_type t;
			if (r < 35) {
				t = SHEEP;
			} else if (r < 36) {
				t = LION;
			} else {
				t = NONE;
			}
			cell[i][j] = t;
		}
	}
}

void print_cells(FILE* fp, int generation) {
	int i, j;
	int sheep_alive_count = 0;
	int lion_alive_count = 0;
	fprintf(fp, "----------\n");

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if ((cell[i][j] == SHEEP)) {
#ifdef _DEBUG
				fprintf(fp, "\033[1;36mw\033[0m");
#else
				fputc('w', fp);
#endif
				sheep_alive_count++;
			} else if (cell[i][j] == LION) {
#ifdef _DEBUG
				fprintf(fp, "\033[1;33m#\033[0m");
#else
				fputc('#', fp);
#endif
				lion_alive_count++;
			} else {
				fputc(' ', fp);
			}
		}
		fputc('\n', fp);
	}
	fflush(fp);
	printf("generation = %d: alive = %d, %d\n", generation, sheep_alive_count, lion_alive_count);
	usleep(100 * 1000);
}

int count_adjacent_cells(int i, int j, enum animal_type t) {
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
			n += (cell[k][l] == t);
		}
	}
	return n;
}

void eat_sheep(int y, int x, enum animal_type next[HEIGHT][WIDTH]) {
	int l = count_adjacent_cells(y, x, LION);
	for (;;) {
		int cx = x + rand() % 3 - 1;
		int cy = y + rand() % 3 - 1;
		if (cx < 0 || cx >= WIDTH || cy < 0 || cy >= HEIGHT) {
			continue;
		}
		if (cell[cy][cx] == SHEEP) {
			next[cy][cx] = LION;
			if (l >= 1) {
				next[y][x] = LION; // breeding
			} else {
				next[y][x] = NONE;
			}
			return;
		}
	}
}

void update_cells() {
	int i, j;
	enum animal_type cell_next[HEIGHT][WIDTH];

	// -- ライオンの行動ターン
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell_next[i][j] = cell[i][j];
		}
	}

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (cell[i][j] == LION) {
				const int s = count_adjacent_cells(i, j, SHEEP);
				if (s >= 1) {
					eat_sheep(i, j, cell_next);
				} else {
					cell_next[i][j] = NONE;
				}
			}
		}
	}

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			cell[i][j] = cell_next[i][j];
		}
	}

	// -- 羊の行動ターン
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			const int n = count_adjacent_cells(i, j, SHEEP);
			if (cell[i][j] == LION) {
				cell_next[i][j] = LION;
				continue;
			}
			switch (n) {
				case 2:
					cell_next[i][j] = cell[i][j];
					break;
				case 3:
				case 4:
					cell_next[i][j] = SHEEP;
					break;
				default:
					cell_next[i][j] = NONE;
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
