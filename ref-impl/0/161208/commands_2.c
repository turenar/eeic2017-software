#include "commands.h"
#include "util.h"

CommandReturn cmd_load(char* args) {
	char* filename = *args == '\0' ? NULL : args;
	CommandReturn result = load_history(filename);
	return result == COMMAND_OK ? COMMAND_SPECIAL : result;
}
