#include <stdio.h>

int main() {
	int c;

	do {
		c = getchar();
		switch (c) {
			case 'g':
			case 'G':
				printf("あなたはグーを出しました。私はチョキを出しました。あなたの勝ち！\n");
				break;
			case 'c':
			case 'C':
				printf("あなたはチョキを出しました。私はパーを出しました。あなたの勝ち！\n");
				break;
			case 'p':
			case 'P':
				printf("あなたはパーを出しました。私はグーを出しました。あなたの勝ち！\n");
				break;
			case 'q':
			case 'Q':
				c = EOF;
				break;
			default:; // do nothing
		}
	} while (c != EOF);
}
