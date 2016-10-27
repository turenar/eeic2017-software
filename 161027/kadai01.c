#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 30
#define HEIGHT 15

/*
 * 教官によると、関数の引数の型が char (*matrix)[nantoka] じゃないとだめみたいなこと言ってましたが、
 * 下のようにchar matrix[HEIGHT][WIDTH]で十分です。
 *
 * main()が長くなると、読むのが嫌になるので、できるだけ関数としてくくり出せるところはくくり出したほうが
 * あとあとになって効いてくると思います。ただ、これはあくまで好みなので、関数で書くより
 * main()の中に書いたほうが好きであれば、そのように。
 */

void init_matrix(char matrix[HEIGHT][WIDTH]);
void update_frame(char matrix[HEIGHT][WIDTH]);
void show_matrix(char matrix[HEIGHT][WIDTH]);


int main() {
	char matrix[HEIGHT][WIDTH];

	init_matrix(matrix);
	for (;;) { // Ctrl+Cするまで無限ループ
		update_frame(matrix);
		show_matrix(matrix);
		sleep(1);
	}
}

void init_matrix(char matrix[HEIGHT][WIDTH]) {
	// FIXME: matrixを初期化する。
}

void update_frame(char matrix[HEIGHT][WIDTH]) {
	static int i = 0;
	// FIXME: (i秒後に→i回目の呼び出しのときに) i列i番目の値が*になるようにする。
	// iを増やすのを忘れずに
}

void show_matrix(char matrix[HEIGHT][WIDTH]) {
	system("clear"); // ←ウィンドウを消去する。
	// FIXME: matrixを表示する
}
