#include <stdio.h>

#define WIDTH 30
#define HEIGHT 15

int main() {
	char matrix[HEIGHT][WIDTH];

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int dx = x - WIDTH / 2;
			int dy = y - HEIGHT / 2;
			if (dx * dx + dy * dy < 25) {
				matrix[y][x] = '*';
			} else {
				matrix[y][x] = '.';
			}
		}
	}

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			printf("%c ", matrix[y][x]);
		}
		putchar('\n');
	}
}
