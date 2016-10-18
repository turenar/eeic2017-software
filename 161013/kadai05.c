#include <stdio.h>

#define NUMFRT 3
#define MAXCHAR 50

int main() {
	char instrument[MAXCHAR] = "Pen";
	char fruit[NUMFRT][MAXCHAR] = {"Apple", "Pineapple", "Appleberry"};

	// 問題文が分かりにくいが、日本語が下手すぎて説明の仕方を思いつかない

	/*
	 * iを0からNUMFRT-1まで増やしながらループ {
	 *   出力("I have a <instrument>.\n");
	 *   出力("I have a <fruit[i]>.\n");
	 *   出力("...Uh, <instrument> <fruit[i]>\n");
	 * }
	 *
	 * iを0からNUMFRT-1まで増やしながらループ {
	 *   出力("<fruit[i]> <instrument>, ");
	 * }
	 *
	 * 出力("\n");
	 * 出力("...Uh, <instrument> ");
	 *
	 * iをNUMFRT-1から0まで減らしながらループ {
	 *   出力("<fruit[i]> ");
	 * }
	 *
	 * 出力("<instrument>!\n");
	 */
	return 0;
}
