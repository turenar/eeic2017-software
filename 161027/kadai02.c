#include <stdio.h>
#include <stdlib.h>

#define WIDTH 30
#define HEIGHT 15

/*
 * 課題1を書き換え，標準入力から
 * % 5 5 *などと入力して表示を書き換えるプログラムを作れ．
 *
 * 指示が分かりにくいので、分かりにくい言葉で説明すると、
 * 5 5 *
 * と入力されたらmatrixの5行5列 (5列5行でもいい) 目を'*'に書き換えるというだけ。
 *
 *
 * 先頭の% は何って？たぶん、scanf呼び出す前にprintf("%% ");してるんじゃないかな。(%が2つになっていることに注意)
 * 「あなたの入力をここに入力してください！」っていう意味で入力待ちする前に"> "とか"% "を出力することは稀によくある。
 */

/**
 * @brief matrixの初期化
 * @param matrix matrix変数をそのまま渡す。&とかはいりません。つまり、mainの中でinit_matrix(matrix);とすれば良い
 */
void init_matrix(char matrix[HEIGHT][WIDTH]);
/**
 * @brief コマンドを受け取る
 *
 * 標準入力 (≒キーボード) から"<x> <y> <c>"の形の文字列を受け取り、それぞれを引数に格納する
 * @param x xを入れるint型変数へのポインタ
 * @param y yを入れるint型変数へのポインタ
 * @param c cを入れるchar型変数へのポインタ
 */
void get_command(int* x, int* y, char* c);
/**
 * @brief 指定した座標を書き換える
 *
 * matrixの(x,y)をcにする
 * @param matrix matrix変数をそのまま渡す。&とかはいらない
 * @param x X座標
 * @param y Y座標
 * @param c 書き換える文字
 */
void update_frame(char matrix[HEIGHT][WIDTH], int x, int y, char c);
/**
 * @brief matrixを表示する
 * @param matrix matrix変数をそのまま渡す。
 */
void show_matrix(char matrix[HEIGHT][WIDTH]);

int main() {
	char matrix[HEIGHT][WIDTH];

	init_matrix(matrix);
	for (;;) { // やっぱり無限ループ
		/* FIXME
		 * get_command
		 * update_frame
		 * show_matrix
		 * をいい感じに呼び出すコードを書く。
		 */
	}
}

void init_matrix(char matrix[HEIGHT][WIDTH]) {
	// FIXME: matrixの初期化
	// 課題1と同じものを使えばよい。
}

void get_command(int* x, int* y, char* c) {
	// FIXME: 書く。
	// ヒント: 講義ページからコピペ。
	//   スペースで区切られた複数のデータを読み込む
	//   int x, y, c;
	//   scanf("%d %d %c", &x, &y, &c);
	//   形式がおかしいと無限ループに陥ることがある。指定した引数の数とscanfの戻り値が一致しない時に入力のバッファをクリアすることで回避する。
	//   詳しくはこちらの最後。
	//   https://ja.wikipedia.org/wiki/Scanf
	// だそうです。
}


void update_frame(char matrix[HEIGHT][WIDTH], int x, int y, char c) {
	// ここは流石に楽勝だと思うので答え書いちゃいますね。
	// % で剰余をとってるので、間違ってxに1000みたいな値を入れても大丈夫？
	matrix[y % HEIGHT][x % WIDTH] = c;
}

void show_matrix(char matrix[HEIGHT][WIDTH]) {
	// FIXME: どうぞあなたの課題1からコピペしてください
}
