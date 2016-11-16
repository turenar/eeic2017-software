#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#define HEIGHT 24
#define WIDTH 39

typedef char life_board_t[HEIGHT][WIDTH];

void init_screen();
void init_board(life_board_t);
void next_generation(life_board_t prev, life_board_t next);
void show_board(life_board_t);
int get_alive_near(int x, int y, life_board_t);
int is_alive(int x, int y, life_board_t);

int main() {
	life_board_t a;
	life_board_t b;
	life_board_t* now = &a;
	life_board_t* next = &b;

	srand((unsigned int) time(NULL));
	init_screen();
	init_board(*now);
	for (;;) {
		show_board(*now);
		next_generation(*now, *next);
		usleep(1000000 / 5);

		life_board_t* tmp = now;
		now = next;
		next = tmp;
	}
}

void init_screen() {
	for (int y = 0; y < HEIGHT; y++) {
		putchar('\n');
	}
}

void init_board(life_board_t board) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			board[y][x] = (rand() & 0x80) != 0; // rand()が線形合同法の場合最終ビットは0と1が交互になりやすい
//			board[y][x] = 0;
		}
	}
	/*board[0][1] = 1;
	board[1][1] = 1;
	board[2][1] = 1;
	board[0][2] = 1;
	board[1][3] = 1;*/
}

void show_board(life_board_t board) {
	printf("\033[3J\033[H\033[2J");
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (is_alive(x, y, board)) {
				printf("\033[47m  \033[0m");
			} else {
				printf("\033[40m  \033[0m");
			}
		}
		putchar('\n');
	}
}

void next_generation(life_board_t prev, life_board_t next) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int c = get_alive_near(x, y, prev);
			char next_alive;
			switch (c) {
				case 3:
					next_alive = 1;
					break;
				case 2:
					next_alive = prev[y][x];
					break;
				default:
					next_alive = 0;
			}
			next[y][x] = next_alive;
		}
	}
}

int get_alive_near(int x, int y, life_board_t board) {
	int count = 0;
	count += is_alive(x - 1, y - 1, board);
	count += is_alive(x - 1, y, board);
	count += is_alive(x - 1, y + 1, board);
	count += is_alive(x + 1, y - 1, board);
	count += is_alive(x + 1, y, board);
	count += is_alive(x + 1, y + 1, board);
	count += is_alive(x, y - 1, board);
	count += is_alive(x, y + 1, board);
	return count;
}

int is_alive(int x, int y, life_board_t board) {
	if (x < 0) {
		x += WIDTH;
	} else if (x >= WIDTH) {
		x -= WIDTH;
	}
	if (y < 0) {
		y += HEIGHT;
	} else if (y >= HEIGHT) {
		y -= HEIGHT;
	}
	return board[y][x];
}
