#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define WIDTH 30
#define HEIGHT 15
#define DATA_FILENAME "data.txt"

/*
 * 講義資料からコピペ
 *   課題3のdata.txtをランレングス法により，データファイルを圧縮して保存せよ．
 *   https://ja.wikipedia.org/wiki/連長圧縮
 *   > 連長圧縮では、ある連続したデータを、そのデータ一つ分と連続した長さで表現することで圧縮している。
 *   > 例えば、「A A A A A B B B B B B B B B A A A」は「A 5 B 9 A 3」と表せる。
 *   > これは、Aが5回続き、そのあとにBが9回、そしてAが3回続いていることを表している
 *   > (連続回数を、元のデータを表す符号の前に記録することもある。その場合、符号化した後は「5 A 9 B 3 A」と表される）。
 *
 * 課題3のコードそのままコピってきて、load_matrixとwrite_matrixにランレングス法関連のコードを書くのが一番早いかな
 *
 * 想定している大まかな流れ: (つまり課題3とほぼ一緒)
 *   * matrixの初期化
 *   * (ファイルが存在すれば) ファイルから読み込み
 *   * †魔の無限る〜ぷ†
 *     * matrixの表示
 *     * "<x> <y> <c>"をキーボードから受け取る
 *     * matrixの(x,y)をcに書き換える
 *     * matrixをファイルを出力
 *
 *
 * x文字の元の文字列にランレングス法をかけると、最長で2x文字になります。
 *
 * どうやってランレングスをかけるかはちょっと迷うところかも。
 * このコードでは、とりあえず
 * [1-9までの数字;繰り返し回数] [繰り返す文字]
 * という形で保存することとします。つまり、
 * AAAAAABBBDD (11文字) → 6A3B2D (6文字)
 * AAAAAAAAAAA (11文字) → 9A2A (4文字)
 * のような、テキストエディタでそのまま開ける形で保存します。
 *
 * あとは、[1-9までの数字を1-4桁;繰り返す回数]<空白>[繰り返す文字]という形でもいいけど、
 * scanf使ったりでめんどくさそう (小学生並みの感想)
 * AAAAAAAAAAA (11文字) → 11 A (4文字)
 *
 * もっと短くしたい！という人は
 * [\x01-\x7fまたは\x01-\xff;繰り返し回数] [繰り返す文字]
 * という形で保存すると127文字または255文字まで省略できる。でも、\x01とか\x7fがどういうことかわからない人は、
 * この形で保存するのはやめたほうがいいと思うよ。
 * AAAAAABBBDD (11文字) → \x06\x41\x03\x42\x02\x43 (6文字)
 * AAAAAAAAAAA (11文字) → \x0b\x41 (2文字)
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

	// FIXME: 課題2からそのままコピってきませう
	// ランレングス関連のコードはload_matrixとwrite_matrixに書いちゃえ〜
}

int file_exists(const char* filename) {
	// http://stackoverflow.com/questions/230062
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

void init_matrix(char matrix[HEIGHT][WIDTH]) {
	// FIXME: 課題2といっしょなのよさ
}

void load_matrix(char matrix[HEIGHT][WIDTH]) {
	// 講義資料からコピペ
	//   入出力に使える関数はこちら
	//   http://www9.plala.or.jp/sgwr-t/c/sec17.html

	FILE* fp = NULL /* FIXME: DATA_FILENAME を読み込み用にオープンする */;
	char data[65536]; // ←こんなに大きな数は要らないので、腕に自身のある人は直してみよう

	/* FIXME: 概念だけ。
	 *
	 * while(まだファイル終端ではない) {
	 *   count = (一文字読み込んで、その文字 (char) を数字 (int) にする。'0'を足したり引いたりする。繰り返し回数)
	 *   c = (一文字読み込む。繰り返す文字)
	 *   dataにcを<count>回追加。
	 * }
	 */

	int i = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			int c = data[i++];
			matrix[y][x] = (char) c;
		}
	}
	// ファイルを閉じる
	fclose(fp);
}

void get_command(int* x, int* y, char* c) {
	// FIXME: 課題2,3と一緒ですにゃ
}

void update_frame(char matrix[HEIGHT][WIDTH], int x, int y, char c) {
	// FIXME: 課題2,3と一緒だよぉ
}

void show_matrix(char matrix[HEIGHT][WIDTH]) {
	// FIXME: やったな、課題2,3と一緒らしいぞ！
}

void write_matrix(char matrix[HEIGHT][WIDTH]) {
	FILE* fp = NULL /* FIXME: DATA_FILENAME 書き込み用にオープンする。その際、ファイルの中身はみんな捨ててしまって良い。 */;

	char prev_char = '\0';
	int count = 0; // prev_charが何文字連続であるか
	int i = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			char c = matrix[y][x];
			if (prev_char != c) {
				// 直前の文字と異なる文字ならば→直前の文字を圧縮して書き込み
				while (1 /* FIXME: countが1以上であるならば*/ ) {
					int repeat; // ←1から9の繰り返し回数
					// FIXME: repeatに9とcountの小さい方を代入する
					fputc('0' + repeat, fp); // repeatが6ならば'6'を出力
					fputc(prev_char, fp);
					// FIXME: 今出力した繰り返し回数分だけcountから引く。つまり、repeat=11, count=9だったらcount=2になるようにする
				}
				// FIXME: prev_charとcountを更新する。
			} else {
				// 直前の文字と同じ文字の場合
				// FIXME: countを1増やす。
			}
		}
	}
	// FIXME: 最後の繰り返し文字もちゃんと出力する。上のwhile文をそのままここにコピペすればいい。

	// ココのfclose重要。理由は課題3で説明したので省略
	fclose(fp);
}
