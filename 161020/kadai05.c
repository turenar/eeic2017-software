#include <stdio.h>
#include <stdlib.h>

#define NUMALPHA 26

int main() {
	char* str;
	int i;
/* mallocによる領域の確保が必要なケース */
	str = /*FIXME: エラー*/ malloc(1 /* FIXME: サイズ */);
/* AからZまでをFor文で入れる */
	for (i = 0; i < NUMALPHA; i++) {
		*(str + i) = i + 'A';
	}
	*(str + i) = '\0';
	printf("Alphabet: %s\n", str);
	return 0;
}

/*
 * mallocするときのサイズはNUMALPHAでいいのでしょうか？
 * Cの文字列は最後に'\0'が入るので、確保する領域は必ず文字数+1以上である必要があります。 (←ヒントというか答え)
 */

/*
 * ついでにどうでもいい話。
 * ポインタ型変数の宣言の流儀は大まかには2つあります。
 *   int *p;
 *   int* p;
 * ぶっちゃけどっちでもいいのですが、
 * 前者は
 *   + int *p, *q; と書くので、複数変数の宣言でも、qがintになる罠に引っかからない
 * 後者は
 *   + intポインタ型であることがわかりやすい
 *   - 複数ポインタ型変数を宣言するときは、int *p; int *q;と分けないと罠に引っかかる
 *     * すべての変数を1行1変数宣言にすれば何の問題もない
 *   * Java書きはこっちのほうが慣れているイメージ (個人の偏見) http://www.ne.jp/asahi/hishidama/home/tech/java/array.html
 * というメリット、デメリットが有ります。繰り返しますが、どちらでも大丈夫です。ただ、一応どちらのコードでも読めるようにしてください。
 */
