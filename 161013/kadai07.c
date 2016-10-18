#include <stdio.h>

#define NALPHA 26

int is_lower_alpha(int c);

int main() {
	int count[NALPHA][NALPHA];

	for (int i = 0; i < NALPHA; i++) {
		for (int j = 0; j < NALPHA; j++) {
			count[i][j] = 0;
		}
	}

	int prev = ' '; // 最初に入れる値はなんでもいいけど、小文字のアルファベットじゃないほうがいい
	int c;
	while ((c = getchar()) != EOF) {
		if(1 /* FIXME: count[][]の値を一つ増やす条件、つまりprevとcが小文字のアルファベットの時 */){
			count[prev- 'a'][c-'a']++;
		}
		prev = c;
	}

	// 表示
	for (int i = 0; i < NALPHA; i++) {
		for (int j = 0; j < NALPHA; j++) {
			printf("[%c][%c] = %d\n", 'a' + i, 'a' + j, count[i][j]);
		}
	}

	return 0;
}

int is_lower_alpha(int c) {
	// FIXME: 課題6と一緒なので省略
	return 0;
}
