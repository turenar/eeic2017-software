#include <stdio.h>
#include <stdlib.h>

void strcpy1(char* dst, const char* src);
size_t strlen1(const char* s);

int main() {
	const char* str = "abcdefhogehogefugafugapiyopiyo";
	char* copied = (char*) malloc(strlen1(str) + 1);

	strcpy1(copied, str);

	printf("original: %s\n", str);
	printf("copied:   %s\n", copied);
}

void strcpy1(char* dst, const char* src) {
	for (; *src != '\0'; src++, dst++) {
		*dst = *src;
	}
	*dst = '\0';
}

size_t strlen1(const char* s) {
	size_t len = 0;

	for (const char* p = s; *p != '\0'; p++) {
		len++;
	}
	return len;
}
