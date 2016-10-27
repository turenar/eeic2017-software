#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>

#define WIDTH 30
#define HEIGHT 15
#define DATA_FILENAME "data.txt"

/*
 * その１
 *   課題2のmatrix[x][y]の内容をその都度data.txtにテキストとして書き出すプログラムを作れ。
 *   データ形式としては，WIDTH*HEIGHT分の文字を一行に収納するようなもので良い．
 * その２
 *   プログラムを起動するたびに、以前に保存したdata.txtの内容を読み込んでmatrix[][]に読み出すようにプログラムを変更せよ．
 *
 * めんどくさいので、その2までのコードがこのファイルになります。
 * 大まかな流れ:
 *   * matrixの初期化
 *   * (ファイルが存在すれば) ファイルから読み込み
 *   * †魔の無限る〜ぷ†
 *     * matrixの表示
 *     * "<x> <y> <c>"をキーボードから受け取る
 *     * matrixの(x,y)をcに書き換える
 *     * matrixをファイルを出力
 *
 * エラーのヒント:
 *   Q. 読み込んだときに<?>のような文字や、四角の中に00ffのような文字が表示される
 *   A. load_matrixの中にちょっとした説明を書いたので読んでちょ
 *
 *   Q. fputcしてるのにファイルが空だよ？
 *   A. 書き込んだらfcloseしてる？load_matrixの中に書いたからry
 */

/**
 * @brief ファイルが存在するかどうかを返す
 * @param filename ファイル名
 * @return ファイルが存在すれば1、存在しなければ0
 */
int file_exists(const char* filename);
/**
 * @brief matrixを初期化する
 * @param matrix matrix変数をそのまま渡す
 */
void init_matrix(char matrix[HEIGHT][WIDTH]);
/**
 * @brief ファイルからmatrixに読み込む
 * @param matrix matrix変数をそのまま渡す
 */
void load_matrix(char matrix[HEIGHT][WIDTH]);
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
void update_frame(char matrix[HEIGHT][WIDTH], int x, int y, char);
/**
 * @brief matrixを表示する
 * @param matrix matrix変数をそのまま渡す。
 */
void show_matrix(char matrix[HEIGHT][WIDTH]);
/**
 * @brief matrixの内容をファイルに書き込む
 * @param matrix matrix変数をそのまま渡す。
 */
void write_matrix(char matrix[HEIGHT][WIDTH]);

int main() {
	char matrix[HEIGHT][WIDTH];

	// FIXME: 動くようにする。どのような処理をするかはファイルはじめのコメント参照
	for (;;) {
		// FIXME: 中身を書く
	}
}

int file_exists(const char* filename) {
	// http://stackoverflow.com/questions/230062
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

void init_matrix(char matrix[HEIGHT][WIDTH]) {
	// FIXME: 課題2といっしょだよ
}

void load_matrix(char matrix[HEIGHT][WIDTH]) {
	// 講義資料からコピペ
	//   入出力に使える関数はこちら
	//   http://www9.plala.or.jp/sgwr-t/c/sec17.html

	FILE* fp = NULL /* FIXME: DATA_FILENAME を読み込み用にオープンする */;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int c = ' ' /* FIXME: fgetcを使ってfpから一文字読み込む */;
			// fgetcで読み込みのときの注意。
			// ファイルを最後まで読み込むとfgetcはEOF (-1) を返します。
			// それをmatrixに入れるとEOF
			matrix[y][x] = (char) c;
		}
	}
	// ファイルを閉じる
	fclose(fp);
}

void get_command(int* x, int* y, char* c) {
	// FIXME: 課題2と一緒なのよ
}

void update_frame(char matrix[HEIGHT][WIDTH], int x, int y, char c) {
	// FIXME: つまるところ、課題2と一緒なのです。
}

void show_matrix(char matrix[HEIGHT][WIDTH]) {
	// FIXME: 課題2と同じコードを書いてほしいですの
}

void write_matrix(char matrix[HEIGHT][WIDTH]) {
	FILE* fp = NULL /* FIXME: DATA_FILENAME 書き込み用にオープンする。その際、ファイルの中身はみんな捨ててしまって良い。 */;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			// FIXME: fputcを使ってmatrixの内容をfpに書き込む
		}
	}
	// ココのfclose重要。書き込み用でファイルを開いたら必ずfcloseする。(読み込みでもちゃんとfcloseしておこう)
	//   難しい話:
	//     1文字1文字書き込むごとに実ファイルを更新すると結構時間がかかってしまうので、
	//     実はfputcしても、実ファイルには書き込まれず、fpと関連付けられたバッファというところに一時的にデータが保管される。
	//     そして、バッファが満杯になるか、fcloseされる、fflushされるのいずれかを満たすと
	//     バッファから実ファイルに書き込まれるようになっている。
	//     なので、書き込んだらfcloseしないと、ファイルに途中までしか書かれていない、
	//     特に小さいデータでは永遠にファイルが空ということになりかねない。
	fclose(fp);
}
