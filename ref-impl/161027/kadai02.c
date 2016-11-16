#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define WIDTH 30
#define HEIGHT 15
#define ERR_NOSTR -1
#define ERR_PARSE -2

typedef char mat_t[HEIGHT][WIDTH];

void prepare_window();
void init_matrix(mat_t);
void get_command(int* x, int* y, char* c);
void update_frame(mat_t, int x, int y, char);
void show_matrix(mat_t);
int next_int(char** str);

int main() {
	mat_t matrix;

	prepare_window();
	init_matrix(matrix);
	for (;;) {
		int x;
		int y;
		char c;
		show_matrix(matrix);
		get_command(&x, &y, &c);
		update_frame(matrix, x, y, c);
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

void get_command(int* x, int* y, char* c) {
	char buf[256];
	printf("\n\033[A\033[K%% "); // 一行下に空行を作る
	for (;;) {
		fgets(buf, sizeof(buf), stdin);

		char* p = buf;
		*x = next_int(&p);
		*y = next_int(&p);
		*c = *p;
		if (*x < 0 || *y < 0) {
			printf("\r\033[Kparse failed, enter <x> <y> <char>\033[A\r\033[K%% ");
		} else {
			printf("\r\033[K");
			break;
		}
	}
	printf("\r\033[%dA", HEIGHT + 1);
}


void update_frame(mat_t matrix, int x, int y, char c) {
	matrix[y % HEIGHT][x % WIDTH] = c;
}

void show_matrix(mat_t matrix) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			printf("%c ", matrix[y][x]);
		}
		putchar('\n');
	}
}

int next_int(char** str) {
	char* start;
	char* end;
	for (start = *str; *start != '\0' && isspace(*start); start++) {
		// 空白以外の文字まで読み進める
	}
	for (end = start; !(*end == '\0' || isspace(*end)); end++) {
		// 空白の文字まで読み進める
	}
	if (start == end) {
		return ERR_NOSTR; // *start==\0: 有効な文字がない
	}
	*end = '\0'; // breaks char

	char* inv;
	int ret = (int) strtol(start, &inv, 10);
	if (inv != end) {
		return ERR_PARSE;
	} else {
		*str = end + 1;
		return ret;
	}
}
