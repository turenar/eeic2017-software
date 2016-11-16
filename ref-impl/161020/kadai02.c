#include <stdio.h>
#include <stdlib.h>

int main() {
	int* p;
	p = (int*) malloc(sizeof(int));
	*p = 100;

	printf("*p = %d\n", *p); // ←pが指し示す先の値
	printf("p = %p\n", p); // ←pが指し示す先のアドレス
	printf("&p = %p\n", &p); // ←p自体のアドレス
}
