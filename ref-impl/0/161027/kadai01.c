#include <stdio.h>
#include <unistd.h>

#define WIDTH 30
#define HEIGHT 15
#define STR1(s) #s
#define STR(s) STR1(s)

typedef char mat_t[HEIGHT][WIDTH];

void prepare_window();
void init_matrix(mat_t);
void update_frame(mat_t);
void show_matrix(mat_t);


int main() {
	mat_t matrix;

	prepare_window();
	init_matrix(matrix);
	for (;;) {
		update_frame(matrix);
		show_matrix(matrix);
		sleep(1);
	}
}

void prepare_window() {
	/*for (int i = 0; i < HEIGHT; i++) {
		putchar('\n');
	}*/
}

void init_matrix(mat_t matrix) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			matrix[y][x] = '.';
		}
	}
}

void update_frame(mat_t matrix) {
	static int i = 0;

	matrix[i % HEIGHT][i % WIDTH] = '*';
	i++;
}

void show_matrix(mat_t matrix) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			printf("%c ", matrix[y][x]);
		}
		putchar('\n');
	}
	printf("\033[" STR(HEIGHT) "A");
}
