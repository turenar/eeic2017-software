#include <stdio.h>

int should_be_nabeatu(int);

int main() {
	for (int i = 1; i <= 1000; ++i) {
		if (should_be_nabeatu(i)) {
			printf("%d!!!\n", i);
		} else {
			printf("%d\n", i);
		}
	}
}

int should_be_nabeatu(int i) {
	/* FIXME
	 * 3の倍数 -> 3で割ったときの余りが0
	 *
	 * 3が付く数字 -> 桁の数字が3
	 *   1の位の数字は i % 10 で得られる。 [1234 % 10] -> 4
	 *   10の位の数字は (i/10) % 10 で得られる。 [(1234/10) % 10] -> [123 % 10] -> 3
	 *   100の位は？
	 *
	 * メモ: 教官の言ってた、whileを使うやつも、上の考え方を発展させただけです。
	 *   1000の位は？10^kの位は？
	 *   ちなみに1万の位は [(123456 / 10 / 10 / 10 / 10) % 10] -> 2
	 */
}
