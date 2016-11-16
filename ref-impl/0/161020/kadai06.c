#include <stdio.h>

int strlen1(const char* s);

int main() {
	const char* str = "abcdefhogehogefugafugapiyopiyo";

	printf("str: %s\n", str);
	printf("len: %d\n", strlen1(str));
}

int strlen1(const char* s) {
	int len = 0;

	for (const char* p = s; *p != '\0'; p++) {
		len++;
	}
	return len;
}
