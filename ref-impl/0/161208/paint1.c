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
		{"line",   cmd_line},
		{"save",   cmd_save},
		{"undo",   cmd_undo},
		{"quit",   cmd_quit},
		{"rect",   cmd_rect},
		{"circle", cmd_circle},
		{NULL, NULL}
};
