#include <stdio.h>
#include <stdlib.h>

#define NUMALPHA 26

int main() {
	char* str;
	int i;

	str = (char*) malloc(NUMALPHA + 1);

	for (i = 0; i < NUMALPHA; i++) {
		*(str + i) = i + 'A';
	}
	*(str + i) = '\0';

	printf("Alphabet: %s\n", str);
	return 0;
}
