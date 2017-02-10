#include "util.h"
#include "list.h"
#include "painter.h"
#include "commands.h"
#include "drawer.h"

#ifndef SEPARATED_SOURCE
#include "painter.c"
#include "list.c"
#include "commands_orig.c"
#include "commands_1.c"
#include "commands_2.c"
#include "commands_3_history.c"
#include "commands_3_set.c"
#include "commands_3_display.c"
#include "commands_3_bmp.c"
#include "commands_3_ppm.c"
#include "commands_3_resize.c"
#include "drawer.c"
#include "util.c"
#include "main.c"
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
