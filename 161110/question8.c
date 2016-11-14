#include <stdio.h>
#include <stdlib.h>

void question8();
int num_of_trials(int seed);
int sum_of_array(int sum[101]);

int main() {
  // Question8
  /*問題が少しわかりにくかったので勘違いしているかも…
     「様々なseedに基づいて乱数を発生させ、その数1-100が全て出揃馬での回数を求め、その平均値を求める。」
     という問題だと解釈しました。
     やっていることとしては、
     1.1から100までの数をそれぞれseedとしてnum_of_trials関数に渡す。
     2.num_of_trials関数では、numというintの配列を作成、まず全ての要素を0で初期化。
     3.rand()で1-100までの数を発生させ（rand（）で発生させる数の範囲をコントロールしたい時は%を使い、rand()が発生させる数を割ったあまりを使用すれば良い）、
     num配列のこの（乱数）番目の値を1に。（その数がすでにハッセしたことを意味する）
     4.1回rand()を使用するごとにsum_of_array()関数を呼び、num配列野中のいくつの数がすでに出現しているかを確認する。
     5.全て出揃う（つまりsum_of_array()が100となった時、それまでにrand()が呼ばれた回数)を返す。
     6.これを繰り返し行い、最後にその平均を返す。
   */
  float sum = 0;
  for (int i = 0; i < 100; i++) {
    sum += num_of_trials(i);
  }
  printf("The mean of trials is %f\n", sum / 100);
}

int num_of_trials(int seed) {
  int num[101];
  for (int i = 1; i < 101; i++) {
    num[i] = 0;
  }
  int i, count = 0, rand_num;
  while (sum_of_array(num) < 100) {
    rand_num = rand() % 100 + 1;
    if (num[rand_num] == 0) {
      num[rand_num] = 1;
    }
    count++;
  }
  return count;
}

int sum_of_array(int num[101]) {
  int sum = 0;
  for (int i = 0; i < 101; i++) {
    sum += num[i];
  }
  return sum;
}
