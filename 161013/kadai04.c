#include <stdio.h>

#define WIDTH 30
#define HEIGHT 15

int main() {
	char matrix[HEIGHT][WIDTH];

	/*
	 * forの条件に注意
	 * 0から始まっているので、y <= HEIGHTではない。
	 *
	 * もし、
	 *   * * * * . . . . . . * * * . . . . . .
	 *   * * * . . . . . . . * * . . . . . . .
	 *   * * . . . . . . . . * . . . . . . . .
	 *   * . . . . . . . . . . . . . . . . . .
	 *   . . . . . . . . . . . . . . . . . . .
	 *   と言った感じで左上以外にも点が打たれているように見えるときは、HEIGHTとWIDTHが逆になっていないか確認してみてください。
	 *   まあ何かわからないことがあったら気軽にslackに投げればいいよ
	 */
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (1 /* FIXME: ここに式を入れる*/) {
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
	return 0;
}
