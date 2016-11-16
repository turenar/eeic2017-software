#include <stdio.h>

int main() {
	const char* firstname = "Yoshihiro";
	char lastname[9] = "Kawahara";

	/* Yoshihiro Kawahara と表示したい */
	printf("Name: %s %s\n", firstname, lastname);

	return 0;
}
