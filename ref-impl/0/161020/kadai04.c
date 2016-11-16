#include <stdio.h>

int main() {
	const char* namelist[] = {"Kawahara", "Nakata", "Sakamoto", "Shinoda"};
	for (int i = 0; i < 4; i++) {
		printf("Name: %s\n", namelist[i]);
	}

	return 0;
}
