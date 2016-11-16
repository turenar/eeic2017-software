#include <stdio.h>
#include <string.h>

int check_kaibun(char* str, size_t len);

int main() {
	char buf[4096];

	fgets(buf, sizeof(buf), stdin);
	size_t len = strlen(buf);
	if (buf[len - 1] == '\n') {
		buf[--len] = '\0';
	}

	if (check_kaibun(buf, len)) {
		printf("`%s' is palindrome.\n", buf);
	} else {
		printf("`%s' is NOT palindrome.\n", buf);
	}
}

int check_kaibun(char* str, size_t len) {
	for (int i = 0, j = len - 1; i < j; ++i, --j) {
		if (str[i] != str[j]) {
			return 0;
		}
	}
	return 1;
}
