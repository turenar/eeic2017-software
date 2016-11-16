#include <stdio.h>

#define NUMFRT 3
#define MAXCHAR 50

int main() {
	char instrument[MAXCHAR] = "Pen";
	char fruit[NUMFRT][MAXCHAR] = {"Apple", "Pineapple", "Appleberry"};

	for (int i = 0; i < NUMFRT; i++) {
		printf("I have a %s.\n", instrument);
		printf("I have a %s.\n", fruit[i]);
		printf("...Uh, %s %s\n\n", fruit[i], instrument);
	}
	for (int i = 0; i < NUMFRT; i++) {
		printf("%s %s, ", fruit[i], instrument);
	}
	printf("\n...Uh, %s ", instrument);
	for (int i = NUMFRT - 1; i >= 0; i--) {
		printf("%s ", fruit[i]);
	}
	printf("%s!\n", instrument);
}
