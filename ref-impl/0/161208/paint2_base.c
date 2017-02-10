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
#include "drawer.c"
#include "util.c"
#include "main.c"
#endif

CommandDecl command_decl[] = {
		{"line",   cmd_line},
		{"save",   cmd_save},
		{"undo",   cmd_undo},
		{"quit",   cmd_quit},
		{"rect",   cmd_rect},
		{"circle", cmd_circle},
		{"load",   cmd_load},
		{NULL, NULL}
};
