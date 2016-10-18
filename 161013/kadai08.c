#include <stdio.h>

#define NALPHA 26

int is_lower_alpha(int c);

int main() {
	//        ↓前の文字↓後ろの文字
	int count[NALPHA][NALPHA];
	int c;

	// count 初期化
	for (int i = 0; i < NALPHA; i++) {
		for (int j = 0; j < NALPHA; j++) {
			count[i][j] = 0;
		}
	}

	// 文字を読み込んでcountしていく
	int prev = ' ';
	while ((c = getchar()) != EOF) {
		if (1 /* FIXME: 課題7と同じ条件。prevとcがどちらも小文字のアルファベット */) {
			count[prev - 'a'][c - 'a']++;
		}
		prev = c;
	}

	// ある文字 x が出現した場合に，次に文字 y が出現する確率 P(y|x) を計算
	double prob[NALPHA][NALPHA];
	for (int i = 0; i < NALPHA; i++) {
		// ある文字 x が出現した場合の全パターンの数
		int sum = 0;

		for (int j = 0; j < NALPHA; j++) {
			/* FIXME: countからsumを更新する */
		}
		for (int j = 0; j < NALPHA; j++) {
			prob[i][j] = (double) count[i][j] / (double) sum;
		}
	}

	for (int i = 0; i < NALPHA; i++) {
		double min = 2.0; // 最初に入れる値はprobの最小値よりも大きい値を。今回はprobが0以上1以下なので1より大きい値を使えば確実
		int minidx = -1; // min == prob[i][j]となるjの値を入れる
		double max = -1.0; // 最初に入れる値はprobの最大値よりも小さい値を。今回はprobが0以上1以下なので0より小さい値を使えば確実
		int maxidx = -1;
		for (int j = 0; j < NALPHA; j++) {
			if (1 /* FIXME: minを更新する必要がある条件。つまり、prob[i][j]がminより小さい時 */ && prob[i][j] > 0.0) {
				min = -1 /* FIXME: prob[i][j] */;
				minidx = -1 /* FIXME */;
			}
			if (1 /* FIXME: maxを更新する必要がある条件 */) {
				max = -1 /* FIXME */;
				maxidx = -1 /* FIXME */;
			}
		}
		// ある文字xが出現したときに、次に来る確率が一番低い文字とその確率、一番高い文字とその確率を表示
		printf("[%c]: min = %c (%e), max = %c (%e)\n",
		       'a' /* FIXME: 文字xを表示するための式を入れる */,
		       'a' /* FIXME: 確率が一番低い文字を表示するための式を入れる。minidxを使う。 */,
		       min, // 一番低い確率
		       'a' /* FIXME: 確率が一番高い文字を表示するための式を入れる。maxidxを使う。 */,
		       max // 一番高い確率
		);
	}

	return 0;
}

int is_lower_alpha(int c) {
	// FIXME: 課題6と一緒なので省略
	return 0;
}
