#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>

#define WIDTH 30
#define HEIGHT 15
#define ERR_NOSTR -1
#define ERR_PARSE -2
#define DATA_FILENAME "data.rle"
typedef char mat_t[HEIGHT][WIDTH];

int file_exists(const char* filename);
void prepare_window();
void init_matrix(mat_t);
void load_matrix(mat_t);
void get_command(int* x, int* y, char* c);
void update_frame(mat_t, int x, int y, char);
void show_matrix(mat_t);
void write_matrix(mat_t);
void uncompress_rle(const char*, char*);
void compress_rle(const char*, char*);
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
		write_matrix(matrix);
	}
}

int file_exists(const char* filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

void prepare_window() {
	/*for (int i = 0; i < HEIGHT; i++) {
		putchar('\n');
	}*/
}

void init_matrix(mat_t matrix) {
	if (file_exists(DATA_FILENAME)) {
		load_matrix(matrix);
	} else {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				matrix[y][x] = '.';
			}
		}
	}
}

void load_matrix(mat_t matrix) {
	FILE* fp = fopen(DATA_FILENAME, "r");
	if (!fp) {
		fprintf(stderr, "open failed: %s\n", DATA_FILENAME);
		abort();
	}

	char buf[4096];
	char uncompressed[HEIGHT * WIDTH + 1];
	fgets(buf, sizeof(buf), fp);
	uncompress_rle(buf, uncompressed);
	char* p = uncompressed;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			char c = *p++;
			if (c == '\0') {
				fprintf(stderr, "load failed: unexpected eof encountered\n");
				abort();
			} else {
				matrix[y][x] = c;
			}
		}
	}
	fclose(fp);
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

void write_matrix(mat_t matrix) {
	char buf[WIDTH * HEIGHT + 1];
	char compressed[WIDTH * HEIGHT * 2 + 1];

	char* p = buf;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			*p++ = matrix[y][x];
		}
	}
	*p = '\0';

	compress_rle(buf, compressed);

	FILE* fp = fopen(DATA_FILENAME, "w");
	if (!fp) {
		fprintf(stderr, "open failed for write: %s\n", DATA_FILENAME);
		abort();
	}
	fputs(compressed, fp);
	fclose(fp);
}

void uncompress_rle(const char* in, char* out) {
	for (; *in != '\0';) {
		int count = *in++; // count != 0
		char c = *in++;
		if (c == '\0') {
			fprintf(stderr, "uncompress_rle: unexpected eof\n");
			abort();
		}
		for (int i = 0; i < count; i++) {
			*out++ = c;
		}
	}
	*out = '\0';
}

void compress_rle(const char* in, char* out) {
	char prev = '\0';
	int count = 0;
	for (; *in != '\0'; in++) {
		char c = *in;
		if (c != prev) {
			if (count != 0) {
				// 最初の文字ではない
				*out++ = (char) count;
				*out++ = prev;
			}
			prev = c;
			count = 1;
		} else {
			if (count == CHAR_MAX) { // unsigned だともうすこし効率が良くなるかもだけど、読み込みchar->unsigned->int変換がめんｄ
				*out++ = (char) count;
				*out++ = prev;
				count = 1;
			} else {
				count++;
			}
		}
	}
	if (count != 0) {
		// 最初の文字ではない
		*out++ = (char) count;
		*out++ = prev;
	}
	*out = '\0';
}
