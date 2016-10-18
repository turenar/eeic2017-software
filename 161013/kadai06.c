#include <stdio.h>

#define NALPHA 26

int is_lower_alpha(int c);

int main() {
	int count[NALPHA];

	for (int i = 0; i < NALPHA; ++i) {
		count[i] = 0;
	}

	int c;
	while ((c = getchar()) != EOF) {
		if (is_lower_alpha(c)) {
			/*
			 * 'a' - 'a' = 0
			 * 'b' - 'a' = 1
			 * 'c' - 'a' = 2
			 */
			count[c - 'a']++; // count[c-'a'] = count[c-'a'] + 1; ということ
		}
	}

	for (int i = 0; i < NALPHA; i++) {
		printf("%c : %d\n", 'a' + i, count[i]);
	}

	return 0;
}

int is_lower_alpha(int c) {
	// 小文字のアルファベットだったら1を返す関数。
	// ASCIIコード表を見てほしいのだけど、aからzまでは 97 ('a') 以上 122 ('z') 以下で判定できる
	// ちなみに、'DEF'のどれかだったら1を返す関数はこんな感じ
	if ('D' <= c && c <= 'F') {
		return 1;
	} else {
		return 0;
	}
}
