#include "commands.h"
#include "util.h"

CommandReturn cmd_history(char* args) {
	UNUSED_ARG(args);
	LinkedListIterator it;
	for (list_iterator(history, &it); list_itr_has_next(&it);) {
		const char* s = list_itr_next(&it);
		printf("%s\n", s);
	}
	return COMMAND_SPECIAL;
}
