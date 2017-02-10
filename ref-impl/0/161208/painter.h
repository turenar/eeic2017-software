#pragma once

#include "list.h"
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
