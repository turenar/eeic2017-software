#pragma once

#include "painter.h"

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
