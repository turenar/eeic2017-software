/* This file is auto-generated */
/* start: util.h */

#include <stdint.h>

#define UNUSED_ARG(var) ((void) var)

/**
 * 空白までをトークンとし、一つのトークンを返す。
 *
 * \code
 * char s[] = "hoge fuga";
 * char* p = s;
 * char* x = tokenize(p, &p); // p -> "fuga"
 * char* y = tokenize(p, &p); // p -> ""
 * char* z = tokenize(p, &p);
 * // x == "hoge" y == "fuga" z == NULL
 * // s == "hoge\0fuga"
 * \endcode
 *
 * @param in 文字列。破壊されるので書き込み可能でなければならない。
 * @param next この次のトークンを得るために使用する文字列。
 * @return NULLでなければ、トークン文字列。
 */
char* tokenize(char* in, char** next);

void remove_newline(char*);

static inline uint8_t get_r(uint32_t);
static inline uint8_t get_g(uint32_t);
static inline uint8_t get_b(uint32_t);

static inline int max(int, int);
static inline int max3(int, int, int);
static inline int min(int, int);
static inline int min3(int, int, int);


static inline uint8_t get_r(uint32_t c) {
	return (uint8_t) ((c & 0xff0000) >> 16);
}

static inline uint8_t get_g(uint32_t c) {
	return (uint8_t) ((c & 0x00ff00) >> 8);
}

static inline uint8_t get_b(uint32_t c) {
	return (uint8_t) ((c & 0x0000ff));
}

static inline int max(const int a, const int b) {
	return (a > b) ? a : b;
}

static inline int max3(const int a, const int b, const int c) {
	return max(a, max(b, c));
}

static inline int min(const int a, const int b) {
	return (a < b) ? a : b;
}

static inline int min3(const int a, const int b, const int c) {
	return min(a, min(b, c));
}
/* end: util.h */
/* start: list.h */

#include <stddef.h>

typedef struct node {
	char* str;
	struct node* next;
	struct node* prev;
} Node;

struct linked_list {
	Node* first;
	Node* last;
	size_t len;
};

struct linked_list_iterator {
	struct linked_list* list;
	struct node* prev; // prev
	struct node* pointer; // now
	struct node* next; // next
};

typedef struct linked_list LinkedList;
typedef struct linked_list_iterator LinkedListIterator;

LinkedList* list_new();
void list_free();

void list_push_front(LinkedList*, const char*);
void list_push_back(LinkedList*, const char*);

void list_pop_front(LinkedList*);
void list_pop_back(LinkedList*);

void list_remove_all(LinkedList*);

size_t list_get_count(LinkedList*);

Node* list_get_back_node(LinkedList*);
// iterator作ったけどあまり意味なかった
void list_iterator(LinkedList*, LinkedListIterator*);
const char* list_itr_next(LinkedListIterator*);
int list_itr_has_next(LinkedListIterator*);
void list_itr_remove(LinkedListIterator*);
/* end: list.h */
/* start: painter.h */

#include <stdint.h>
#include <stdio.h>


typedef enum {
	COMMAND_OK, COMMAND_SPECIAL, COMMAND_ERROR, COMMAND_UNKNOWN, COMMAND_EXIT
} CommandReturn;

typedef CommandReturn (* CommandFunc)(char*);

typedef struct {
	const char* name;
	CommandFunc fn;
} CommandDecl;


// needs to define
extern CommandDecl command_decl[];

extern int height;
extern int width;
extern uint32_t fg_color;
extern uint32_t bg_color;
extern int fill;
extern uint32_t** canvas;
extern LinkedList* history;

void print_canvas(FILE* fp);
void init_canvas(int w, int h);
void set_canvas_color_safe(int x, int y, uint32_t color);
CommandReturn interpret_command(const char* cmd);

void draw_line(int x0, int y0, int x1, int y1);

CommandReturn save_history(const char* filename);
CommandReturn load_history(const char* filename);
/* end: painter.h */
/* start: commands.h */


#define INVALID_COLOR 0xffffffff
#define INCH_PER_METER 39.3701
// orig
CommandReturn cmd_line(char*);
CommandReturn cmd_save(char*);
CommandReturn cmd_undo(char*);
CommandReturn cmd_quit(char*);

// for 1
CommandReturn cmd_rect(char*);
CommandReturn cmd_circle(char*);

// for 2
CommandReturn cmd_load(char*);

// for 3
CommandReturn cmd_history(char*);

CommandReturn cmd_fg(char*);
CommandReturn cmd_bg(char*);
CommandReturn cmd_fill(char*);
CommandReturn cmd_resize(char* args);

CommandReturn cmd_show(char*);

CommandReturn cmd_bmp(char*);
CommandReturn cmd_ppm(char*);
/* end: commands.h */
/* start: drawer.h */
void draw_line(int x0, int y0, int x1, int y1);
void draw_circle(int x, int y, int r);
void draw_rect(int x0, int y0, int x1, int y1);
/* end: drawer.h */

#ifndef SEPARATED_SOURCE
/* start: painter.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define BUFSIZE 1024

LinkedList* history;
uint32_t** canvas;

int height;
int width;
uint32_t fg_color = 0xffffffu;
uint32_t bg_color = 0x000000u;
int fill = 0;

const char* default_history_file = "history.txt";

void print_canvas(FILE* fp) {
	int x, y;

	fprintf(fp, "----------\n");

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			const char c = (char) (canvas[y][x] ? '#' : ' ');
			fputc(c, fp);
		}
		fputc('\n', fp);
	}
	fflush(fp);
}

void init_canvas(int w, int h) {
	canvas = (uint32_t**) malloc(sizeof(uint32_t*) * (size_t) h);
	for (int i = 0; i < h; ++i) {
		canvas[i] = (uint32_t*) calloc((size_t) w, sizeof(uint32_t));
	}
	width = w;
	height = h;
}

void set_canvas_color_safe(int x, int y, uint32_t color) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		canvas[y][x] = color;
	}
}

CommandReturn save_history(const char* filename) {
	if (filename == NULL) {
		filename = default_history_file;
	}

	FILE* fp;
	if ((fp = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "error: cannot open %s: %s\n", filename, strerror(errno));
		return COMMAND_ERROR;
	}

	LinkedListIterator it;
	for (list_iterator(history, &it); list_itr_has_next(&it);) {
		const char* s = list_itr_next(&it);
		fprintf(fp, "%s\n", s);
	}

	printf("saved as \"%s\"\n", filename);
	fclose(fp);
	return COMMAND_OK;
}

CommandReturn load_history(const char* filename) {
	if (filename == NULL) {
		filename = default_history_file;
	}

	FILE* fp;
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "error: cannot open %s: %s\n", filename, strerror(errno));
		return COMMAND_ERROR;
	}

	char buf[1024];
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		remove_newline(buf);
		interpret_command(buf);
		list_push_back(history, buf);
	}
	return COMMAND_OK;
}

// Interpret and execute a command
//   return value:
//     0, normal commands such as "line"
//     1, unknown or special commands (not recorded in history[])
//     2, quit
CommandReturn interpret_command(const char* command) {
	char buf[BUFSIZE];
	strcpy(buf, command);

	char* arg;
	const char* cname = tokenize(buf, &arg);
	if (cname == NULL) {
		return COMMAND_UNKNOWN;
	}

	for (CommandDecl* decl = command_decl; decl->name != NULL; ++decl) {
		if (strcmp(decl->name, cname) == 0) {
			return decl->fn(arg);
		}
	}

	fprintf(stderr, "error: unknown command: %s\n", cname);
	return COMMAND_UNKNOWN;
}
/* end: painter.c */
/* start: list.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXLEN 1024

static Node* list_node_new(const char*);
static void list_node_free(Node*);

static Node* list_node_new(const char* str) {
	Node* p = (Node*) calloc(1, sizeof(Node));
	char* s = (char*) malloc(strlen(str) + 1);
	strcpy(s, str);
	p->str = s;
	return p;
}

static void list_node_free(Node* n) {
	free(n->str);
	free(n);
}

LinkedList* list_new() {
	LinkedList* ret = (LinkedList*) malloc(sizeof(LinkedList));
	memset(ret, 0, sizeof(LinkedList));
	return ret;
}

void list_push_front(LinkedList* list, const char* str) {
	// Create a new element
	Node* p = list_node_new(str);
	p->next = list->first;
	if (list->first) {
		list->first->prev = p;
	} else {
		list->last = p;
	}
	list->first = p;
	list->len++;
}

void list_push_back(LinkedList* list, const char* str) {
	// Create a new element
	Node* p = list_node_new(str);
	p->prev = list->last;
	if (list->last) {
		list->last->next = p;
	} else {
		list->first = p;
	}
	list->last = p;
	list->len++;
}

void list_pop_front(LinkedList* list) {
	assert(list->first != NULL); // Don't call pop_front() when the list is empty
	Node* p = list->first;
	list->first = p->next;
	if (!p->next) {
		list->last = NULL;
	}

	free(p->str);
	free(p);
	list->len--;
}

void list_pop_back(LinkedList* list) {
	assert(list->last != NULL); // Don't call pop_back() when the list is empty
	Node* p = list->last;
	list->last = p->prev;
	if (!p->prev) {
		list->first = NULL;
	}

	free(p->str);
	free(p);
	list->len--;
}

void list_remove_all(LinkedList* list) {
	Node* p = list->first;
	while (p != NULL) {
		Node* next = p->next;
		list_node_free(p);
		p = next;
	}
	list->len = 0;
}

size_t list_get_count(LinkedList* list) {
	return list->len;
}

void list_iterator(LinkedList* list, LinkedListIterator* it) {
	it->list = list;
	it->pointer = NULL;
	it->next = list->first;
}

const char* list_itr_next(LinkedListIterator* it) {
	assert(it->next);
	Node* p = it->next;
	it->next = p->next;
	return p->str;
}

int list_itr_has_next(LinkedListIterator* it) {
	return it->next != NULL;
}

void list_itr_remove(LinkedListIterator* it) {
	Node* p = it->pointer;
	if (p->prev) {
		p->prev->next = p->next;
	}
	if (p->next) {
		p->next->prev = p->prev;
	}
	list_node_free(p);
	it->pointer = NULL;
	it->list->len--;
}

Node* list_get_back_node(LinkedList* list) {
	return list->last;
}
/* end: list.c */
/* start: commands_orig.c */
#include <string.h>
#include <stdlib.h>

CommandReturn cmd_line(char* args) {
	int x0, y0, x1, y1;
	char* endptr;
	x0 = (int) strtol(args, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	y0 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	x1 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	y1 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != '\0') {
		goto usage;
	}

	draw_line(x0, y0, x1, y1);
	return COMMAND_OK;

usage:
	fprintf(stderr, "usage: square <x0> <y0> <x1> <y1>\nall arguments must be int.\n");
	return COMMAND_ERROR;
}

CommandReturn cmd_save(char* arg) {
	CommandReturn result = save_history(*arg == '\0' ? NULL : arg);
	return result == COMMAND_OK ? COMMAND_SPECIAL : result;
}

CommandReturn cmd_undo(char* args) {
	UNUSED_ARG(args);
	list_pop_back(history);

	init_canvas(width, height);

	LinkedListIterator it;
	for (list_iterator(history, &it); list_itr_has_next(&it);) {
		const char* s = list_itr_next(&it);
		interpret_command(s);
	}
	return COMMAND_SPECIAL;
}

CommandReturn cmd_quit(char* args) {
	UNUSED_ARG(args);
	return COMMAND_EXIT;
}
/* end: commands_orig.c */
/* start: commands_1.c */
#include <stdlib.h>
#include <string.h>

CommandReturn cmd_rect(char* args) {
	int x0, y0, x1, y1;
	char* endptr;
	x0 = (int) strtol(args, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	y0 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	x1 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	y1 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != '\0') {
		goto usage;
	}

	draw_rect(x0, y0, x1, y1);
	return COMMAND_OK;

usage:
	fprintf(stderr, "usage: square <x0> <y0> <x1> <y1>\nall arguments must be int.\n");
	return COMMAND_ERROR;
}

CommandReturn cmd_circle(char* args) {
	int x0, y0, r;
	char* endptr;
	x0 = (int) strtol(args, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	y0 = (int) strtol(endptr, &endptr, 10);
	if (*endptr != ' ') {
		goto usage;
	}
	r = (int) strtol(endptr, &endptr, 10);
	if (*endptr != '\0') {
		goto usage;
	}

	draw_circle(x0, y0, r);
	return COMMAND_OK;

usage:
	fprintf(stderr, "usage: circle <x> <y> <r>\nall arguments must be int.\n");
	return COMMAND_ERROR;
}


/* end: commands_1.c */
/* start: commands_2.c */

CommandReturn cmd_load(char* args) {
	char* filename = *args == '\0' ? NULL : args;
	CommandReturn result = load_history(filename);
	return result == COMMAND_OK ? COMMAND_SPECIAL : result;
}
/* end: commands_2.c */
/* start: commands_3_history.c */

CommandReturn cmd_history(char* args) {
	UNUSED_ARG(args);
	LinkedListIterator it;
	for (list_iterator(history, &it); list_itr_has_next(&it);) {
		const char* s = list_itr_next(&it);
		printf("%s\n", s);
	}
	return COMMAND_SPECIAL;
}
/* end: commands_3_history.c */
/* start: commands_3_set.c */
#include <string.h>
#include <stdlib.h>

static uint32_t parse_css_color(const char* color);
static uint32_t parse_hex_color(const char* color);
static CommandReturn set_color(const char* arg, uint32_t* to);

static uint32_t parse_css_color(const char* color) {
#define CHECK_CSS_COLOR(_hex_, _name_) do { if (strcmp(_name_, color) == 0) { return _hex_; } } while(0)
	CHECK_CSS_COLOR(0x000000, "black");
	CHECK_CSS_COLOR(0x808080, "gray");
	CHECK_CSS_COLOR(0xc0c0c0, "silver");
	CHECK_CSS_COLOR(0xffffff, "white");
	CHECK_CSS_COLOR(0x0000ff, "blue");
	CHECK_CSS_COLOR(0x000080, "navy");
	CHECK_CSS_COLOR(0x008080, "teal");
	CHECK_CSS_COLOR(0x008000, "green");
	CHECK_CSS_COLOR(0x00ff00, "lime");
	CHECK_CSS_COLOR(0x00ffff, "aqua");
	CHECK_CSS_COLOR(0xffff00, "yellow");
	CHECK_CSS_COLOR(0xff0000, "red");
	CHECK_CSS_COLOR(0xff00ff, "fuchsia");
	CHECK_CSS_COLOR(0x808000, "olive");
	CHECK_CSS_COLOR(0x800080, "purple");
	CHECK_CSS_COLOR(0x800000, "maroon");

	fprintf(stderr, "invalid css color name: %s\n", color);
	return INVALID_COLOR;
}


static uint32_t parse_hex_color(const char* color) {
	size_t len = strlen(color);
	char* endptr;
	uint32_t c = (uint32_t) strtoul(color, &endptr, 16);
	if (*endptr != '\0') {
		fprintf(stderr, "invalid hex: %s\n", color);
		return INVALID_COLOR;
	}
	if (len == 3) {
		uint32_t r = (c & 0xf00) >> 8;
		uint32_t g = (c & 0x0f0) >> 4;
		uint32_t b = (c & 0x00f);
		c = (r << 20) | (r << 16) | (g << 12) | (g << 8) | (b << 4) | b;
	} else if (len == 6) {
		// do nothing
	} else {
		fprintf(stderr, "invalid color hex (not 0xHEX nor 0xHEXHEX): %s\n", color);
		return INVALID_COLOR;
	}
	return c;
}

static CommandReturn set_color(const char* arg, uint32_t* to) {
	uint32_t c;
	if (arg[0] == '\0') {
		printf("#%06x\n", *to);
		return COMMAND_SPECIAL;
	} else if (arg[0] == '#') {
		c = parse_hex_color(arg + 1);
	} else if (arg[0] == '0' && arg[1] == 'x') {
		c = parse_hex_color(arg + 2);
	} else {
		c = parse_css_color(arg);
	}
	if (c == INVALID_COLOR) {
		return COMMAND_ERROR;
	} else {
		*to = c;
		return COMMAND_OK;
	}
}

CommandReturn cmd_fg(char* args) {
	return set_color(args, &fg_color);
}

CommandReturn cmd_bg(char* args) {
	return set_color(args, &bg_color);
}

CommandReturn cmd_fill(char* args) {
	if (args[0] == '\0') {
		printf("fill: %s\n", fill ? "yes" : "no");
		return COMMAND_SPECIAL;
	}
	if (strcmp(args, "y") == 0 || strcmp(args, "yes") == 0 || strcmp(args, "true") == 0) {
		fill = 1;
	} else if (strcmp(args, "n") == 0 || strcmp(args, "no") == 0 || strcmp(args, "false") == 0) {
		fill = 0;
	} else {
		printf("usage: fill <y|n>\n");
		return COMMAND_ERROR;
	}
	return COMMAND_OK;
}
/* end: commands_3_set.c */
/* start: commands_3_display.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	DISPLAY_2COLORS,
	DISPLAY_16COLORS,
	DISPLAY_256COLORS,
	DISPLAY_24BITS
} display_color_type;

static display_color_type display_type;

static CommandReturn show_2c();
static CommandReturn show_16c();
static int to_256c_rgb_unit(int);
static CommandReturn show_256c();
static CommandReturn show_24b();

CommandReturn cmd_show(char* args) {
	if (args[0] != '\0') {
		if (strcmp("2", args) == 0) {
			display_type = DISPLAY_2COLORS;
		} else if (strcmp("16", args) == 0) {
			display_type = DISPLAY_16COLORS;
		} else if (strcmp("256", args) == 0) {
			display_type = DISPLAY_256COLORS;
		} else if (strcmp("24b", args) == 0) {
			display_type = DISPLAY_24BITS;
		} else {
			fprintf(stderr, "unknown display type: %s\n", args);
			fprintf(stderr, "supported: 2 16 256 24b\n");
			return COMMAND_ERROR;
		}
	}

	switch (display_type) {
		case DISPLAY_2COLORS:
			return show_2c();
		case DISPLAY_16COLORS:
			return show_16c();
		case DISPLAY_256COLORS:
			return show_256c();
		case DISPLAY_24BITS:
			return show_24b();
		default:
			abort();
	}
}

static CommandReturn show_2c() {
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			if (*col_p++) {
				putchar('#');
			} else {
				putchar(' ');
			}
		}
		putchar('\n');
	}
	return COMMAND_SPECIAL;
}

static CommandReturn show_16c() {
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			int r = ((c & 0xff0000) >> 16) >= 128u;
			int g = (c & 0x00ff00) >> 8 >= 128u;
			int b = (c & 0x0000ff) >= 128u;
			int sp = r + (g << 1) + (b << 2);
			printf("\033[4%dm \033[0m", sp);
		}
		putchar('\n');
	}
	return COMMAND_SPECIAL;
}

static int to_256c_rgb_unit(int c) {
	// unit: 0   1   2   3   4   5
	// hex: 00  5f  87  af  d7  ff
	// dec: 00  95 135 175 215 255
	if (c < 55) {
		return 0;
	} else {
		return (c - 16) / 40;
	}
}

static CommandReturn show_256c() {
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			int r = (int) (c & 0xff0000) >> 16;
			int g = (int) (c & 0x00ff00) >> 8;
			int b = (int) (c & 0x0000ff);
			// 彩度
			int i_min = min3(r, g, b);
			int i_max = max3(r, g, b);
			int s = i_max == 0 ? 0 : ((i_max - i_min) * 256 / i_max);
			int sp;
			if (s < 32) {
				// quote from https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
				// grayscale from black to white in 24 steps
				sp = 232 + (i_max + i_min) * 24 / 510;
			} else {
				sp = 16 + 36 * to_256c_rgb_unit(r) + 6 * to_256c_rgb_unit(g) + to_256c_rgb_unit(b);
			}
			printf("\033[48;5;%dm \033[0m", sp);
		}
		putchar('\n');
	}
	return COMMAND_SPECIAL;
}

static CommandReturn show_24b() {
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			int r = (int) (c & 0xff0000) >> 16;
			int g = (int) (c & 0x00ff00) >> 8;
			int b = (int) (c & 0x0000ff);
			printf("\033[48;2;%d;%d;%dm \033[0m", r, g, b);
		}
		putchar('\n');
	}
	return COMMAND_SPECIAL;
}
/* end: commands_3_display.c */
/* start: commands_3_bmp.c */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BITMAP_FILE_HEADER_REAL_SIZE 14
struct bitmap_info_header {
	uint32_t header_size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression;
	uint32_t image_data_size;
	int32_t x_pix_per_meter;
	int32_t y_pix_per_meter;
	uint32_t used_palette_size;
	uint32_t important_color_count;
};

struct bitmap_file_header {
	char file_type[2];
	uint32_t file_size;
	uint32_t reserved;
	uint32_t offset_bytes;
	struct bitmap_info_header info;
};

static void write_bitmap_header(struct bitmap_file_header* header, FILE* fp);

static void write_bitmap_header(struct bitmap_file_header* header, FILE* fp) {
	// little-endian
	fwrite(header, 2, 1, fp);
	// offsetof(,file_size) should be 4 but bmp must be packed
	void* remain_header = ((char*) header) + offsetof(struct bitmap_file_header, file_size);
	fwrite(remain_header, 4, 3, fp);
	fwrite(&header->info, header->info.header_size, 1, fp);
}

CommandReturn cmd_bmp(char* filename) {
	if (filename[0] == '\0') {
		fprintf(stderr, "usage: bmp <filename>\n");
		return COMMAND_ERROR;
	}

	struct bitmap_file_header header;
	memset(&header, 0, sizeof(header));
	header.file_type[0] = 'B';
	header.file_type[1] = 'M';
	header.info.header_size = sizeof(header.info);
	header.offset_bytes = BITMAP_FILE_HEADER_REAL_SIZE + header.info.header_size;
	header.info.width = width;
	header.info.height = height;
	header.info.planes = 1;
	header.info.bit_count = 32; // 行間paddingがめんどくさい
	header.info.compression = 0;
	header.info.image_data_size = (uint32_t) (4 * width * height);
	header.info.x_pix_per_meter = (int32_t) (12 * INCH_PER_METER);
	header.info.y_pix_per_meter = (int32_t) (12 * INCH_PER_METER);
	header.info.used_palette_size = 0;
	header.info.important_color_count = 0;
	header.file_size = header.offset_bytes + header.info.image_data_size;

	FILE* fp = fopen(filename, "w");
	if (!fp) {
		perror(filename);
		return COMMAND_ERROR;
	}

	write_bitmap_header(&header, fp);
	for (int y = height - 1; y >= 0; --y) {
		uint32_t* col_p = canvas[y];
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			uint8_t bgra[4];
			bgra[0] = get_b(c);
			bgra[1] = get_g(c);
			bgra[2] = get_r(c);
			bgra[3] = 0u;
			fwrite(bgra, 1, 4, fp);
		}
	}
	fclose(fp);
	printf("bmp file written: %s\n", filename);
	return COMMAND_OK;
}
/* end: commands_3_bmp.c */
/* start: commands_3_ppm.c */
#include <stdio.h>

CommandReturn cmd_ppm(char* filename) {
	if (filename[0] == '\0') {
		fprintf(stderr, "usage: bmp <filename>\n");
		return COMMAND_ERROR;
	}

	FILE* fp = fopen(filename, "w");
	if (!fp) {
		perror(filename);
		return COMMAND_ERROR;
	}

	fprintf(fp, "P3\n%d %d\n255\n", width, height);
	uint32_t** row_p = canvas;
	for (int y = 0; y < height; ++y) {
		uint32_t* col_p = *row_p++;
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			fprintf(fp, "%d %d %d\n", get_r(c), get_g(c), get_b(c));
		}
	}
	fclose(fp);
	printf("ppm file written: %s\n", filename);
	return COMMAND_OK;
}

/* end: commands_3_ppm.c */
/* start: commands_3_resize.c */
#include <stdlib.h>

CommandReturn cmd_resize(char* args) {
	// w h
	char* endptr;
	int w, h;
	w = (int) strtol(args, &endptr, 10);
	if (*endptr != ' ') {
		goto print_usage;
	}
	h = (int) strtol(endptr, &endptr, 10);
	if (*endptr != '\0') {
		goto print_usage;
	}
	goto parse_success;

print_usage:
	fprintf(stderr, "usage: resize <width> <height> [<x0> <y0>]\n");
	return COMMAND_ERROR;

parse_success:
	(void) 0; // error: ラベルは文または宣言の一部にのみなることができます
	uint32_t** old_canvas = canvas;
	int old_w = width;
	int old_h = height;

	init_canvas(w, h);

	int max_w, max_h;
	max_w = min(w, old_w);
	max_h = min(h, old_h);
	
	uint32_t** out_row_p = canvas;
	uint32_t** in_row_p = old_canvas;
	for (int y = 0; y < max_h; ++y) {
		uint32_t* out_col_p = *out_row_p++;
		uint32_t* in_col_p = *in_row_p++;
		for (int x = 0; x < max_w; ++x) {
			*out_col_p++ = *in_col_p++;
		}
	}

	uint32_t** row_p = old_canvas;
	for (int y = 0; y < old_h; ++y) {
		free(*row_p++);
	}
	free(old_canvas);

	return COMMAND_OK;
}
/* end: commands_3_resize.c */
/* start: drawer.c */
#include <stdlib.h>
#include <assert.h>


static inline void swap_int(int*, int*);

static void swap_int(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}


void draw_line(const int x0, const int y0, const int x1, const int y1) {
	int i;
	const int n = max(abs(x1 - x0), abs(y1 - y0));
	for (i = 0; i <= n; i++) {
		const int x = x0 + i * (x1 - x0) / n;
		const int y = y0 + i * (y1 - y0) / n;
		set_canvas_color_safe(x, y, fg_color);
	}
}

void draw_circle(const int x0, const int y0, const int r) {
	assert(r >= 0);

	int r2 = r * r;
	int dy = r;
	for (int dx = 0; dx <= r && dy >= 0;) {
		if (dx * dx + dy * dy >= r2) {
			set_canvas_color_safe(x0 + dx, y0 + dy, fg_color);
			set_canvas_color_safe(x0 + dx, y0 - dy, fg_color);
			set_canvas_color_safe(x0 - dx, y0 + dy, fg_color);
			set_canvas_color_safe(x0 - dx, y0 - dy, fg_color);
		}
		int dx_1 = dx + 1;
		int dy_1 = dy - 1;
		if (dx * dx + dy_1 * dy_1 >= r2) {
			--dy;
		} else if (dx_1 * dx_1 + dy_1 * dy_1 >= r2) {
			++dx;
			--dy;
		} else {
			++dx;
		}
	}
	if (fill) {
		for (int dx = 0; dx <= r; ++dx) {
			for (dy = 0; dy <= r; ++dy) {
				if (dx * dx + dy * dy < r2) {
					set_canvas_color_safe(x0 + dx, y0 + dy, bg_color);
					set_canvas_color_safe(x0 + dx, y0 - dy, bg_color);
					set_canvas_color_safe(x0 - dx, y0 + dy, bg_color);
					set_canvas_color_safe(x0 - dx, y0 - dy, bg_color);
				}
			}
		}
	}
}

void draw_rect(int x0, int y0, int x1, int y1) {
	if (x0 > x1) {
		swap_int(&x0, &x1);
	}
	if (y0 > y1) {
		swap_int(&y0, &y1);
	}
	int x_max = min(x1, width - 1);
	for (int x = x0; x <= x_max; ++x) {
		set_canvas_color_safe(x, y0, fg_color);
		set_canvas_color_safe(x, y1, fg_color);
	}
	int y_max = min(y1, height - 1);
	for (int y = y0; y <= y_max; ++y) {
		set_canvas_color_safe(x0, y, fg_color);
		set_canvas_color_safe(x1, y, fg_color);
	}
	if (fill) {
		for (int x = x0 + 1; x < x_max; ++x) {
			for (int y = y0 + 1; y < y_max; ++y) {
				canvas[y][x] = bg_color;
			}
		}
	}
}
/* end: drawer.c */
/* start: util.c */
#include <string.h>

char* tokenize(char* in, char** next) {
	char* p;
	for (p = in; *p != '\0'; ++p) {
		if (*p == ' ') {
			if (p == in) {
				in = p + 1;
			} else {
				*p = '\0';
				*next = p + 1;
				return in;
			}
		}
	}
	if (p != in) {
		*next = p; // '\0'
		return in;
	} else {
		return NULL;
	}
}

void remove_newline(char* str) {
	char* endptr = &str[strlen(str) - 1];
	if (*endptr == '\r' || *endptr == '\n') {
		*endptr = '\0'; // remove the line delimiter at the end
	}
}
/* end: util.c */
/* start: main.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef DEFAULT_WIDTH
#define DEFAULT_WIDTH 70
#endif
#ifndef DEFAULT_HEIGHT
#define DEFAULT_HEIGHT 40
#endif

#define BUFSIZE 1024

static int batch_main(const char* filename);
static int interactive_loop();

static int batch_main(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		perror(filename);
		return 1;
	}

	history = list_new();
	init_canvas(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	char buf[BUFSIZE];
	int has_error = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		remove_newline(buf);
		const int result = interpret_command(buf);
		switch (result) {
			case COMMAND_EXIT:
				goto exit_label;
			case COMMAND_OK:
			case COMMAND_SPECIAL:
				list_push_back(history, buf);
				break;
			default:
				has_error = 1;
		}
	}
exit_label:
	return has_error;
}

static int interactive_loop() {

	const char* filename = "canvas.txt";
	FILE* fp;
	char buf[BUFSIZE];

	if ((fp = fopen(filename, "a")) == NULL) {
		fprintf(stderr, "error: cannot open %s.\n", filename);
		return 1;
	}

	history = list_new();
	init_canvas(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	print_canvas(fp);
	do {
		printf("%u> ", (unsigned int) list_get_count(history));
		if (fgets(buf, BUFSIZE, stdin) == NULL) {
			printf("\nexit ok? [y/N]> ");
			if (fgets(buf, BUFSIZE, stdin) == NULL) {
				break;
			}
			remove_newline(buf);
			if (strcmp(buf, "y") == 0 || strcmp(buf, "yes") == 0) {
				break;
			} else if (*buf == '\0' || strcmp(buf, "n") == 0 || strcmp(buf, "no") == 0) {
				// valid answer
			} else {
				fprintf(stderr, "answer with y or n!\n");
			}
			continue;
		}

		remove_newline(buf);
		const int r = interpret_command(buf);
		if (r == COMMAND_EXIT) {
			break;
		}
		if (r == COMMAND_OK) {
			list_push_back(history, buf);
		}
		print_canvas(fp);
	} while (1);

	fclose(fp);
	return 0;
}

int main(int argc, char** argv) {
	if (argc >= 2) {
		return batch_main(argv[1]);
	} else {
		return interactive_loop();
	}
}
/* end: main.c */
#endif

CommandDecl command_decl[] = {
		{"line",     cmd_line},
		{"save",     cmd_save},
		{"undo",     cmd_undo},
		{"quit",     cmd_quit},
		{"rect",     cmd_rect},
		{"circle",   cmd_circle},
		{"load",     cmd_load},
		{"history",  cmd_history},
		{"fg",       cmd_fg},
		{"bg",       cmd_bg},
		{"fill",     cmd_fill},
		{"show",     cmd_show},
		{"resize",   cmd_resize},
		{"bmp",      cmd_bmp},
		{"ppm",      cmd_ppm},
		{NULL, NULL}
};
