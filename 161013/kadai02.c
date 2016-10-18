#include <stdio.h>

int main() {
	int c;
	int quit = 0; // 終了するときには1にする


	while (!quit && (c = getchar()) != EOF) {
		switch (c) { // ifでも大丈夫。
			case 'g':
			case 'G': // 小文字大文字両方に対応する
				printf("ここにメッセージを入れる");
				break;

				// 以下c,pについても同様
			case 'q':
			case 'Q':
				quit = 1;
				break;
			default: // '\n'などはここに流れる
				; // do nothing
		}
	}
}
