/*授業ページを見ながら進めてください。
   https://sites.google.com/a/akg.t.u-tokyo.ac.jp/soft1/lecture-7
 */
#include <stdio.h>
#include <stdlib.h>

void question6();
int is_prime(int num);
void question6_era();
void question8();
int num_of_trials(int seed);
int sum_of_array(int sum[101]);

int main() {

  // Question1
  char *name = "Todai";
  printf("Name=%s\n", name); // output:Name=Todai
  /*ここでnameのかわりに*nameとしていけないのはなぜでしょう？
     %sはprintf内で文字列（char配列）をパラメータとしてとっていることに注意。
   */
  printf("Name=%c\n", *name); // output: Name=T
  /*%cになっていることに注目。
     つまりここではnameポインタがさすもともとのString（char配列）全体ではなく、
     配列の先頭のcharを指し示しているだけ。
   */

  // question2
  int i = 5;
  printf("%d\n", i == 5); // output:1,
  /* ここで==は比較演算子。つまり「iが5」ならば1(true)を返し、ちがえば0をかえす。
     ここでは一つ上の行でiが5で定義されているのでi==5は正しく、1が返される。

      1 == 1 --> 1 (右辺と左辺が同じ、つまり 1 == 1と言う式が正しいため。)
      1 == 0 --> 0 (1と0は同じではないので 1==0という式は誤り。よって0を返す。)

     ちなみにcではBOOL型が標準では存在しない（!）ので、true/falseの結果は1か0で帰ってきます。
     C99以降ならば #include <stdbool.h>すればtrue,
     false使えますが、テストではintの0,1で判別すれば十分だと思います。
   */

  // question3
  int *a, b = 10;
  a = &b;
  printf("%d\n", *a); // output: 10
  //ちなみにもちろん以下もオッケーですが問題の趣旨的に多分ポインタを使う方が正しいかと。
  printf("%d\n", *a); // output: 10

  // question4
  printf("%c\n", ("hello"[1] - 1)); // output:d
  /*
     "hello"[1]
     は"hello"というchar配列の2番目（注）の要素、つまり'e'を返します。
     char'd'から-1しているので,ASCIIテーブルでcharの一つまえ、つまり小文字のdが帰ってきます。
     ASCII表: http://www9.plala.or.jp/sgwr-t/c_sub/ascii.html
     （注）Cは0-indexing、つまり0から数え上げが始まります。うっかり最初の要素だと思いわないように注意。
   */

  // Question5
  char *array[5] = {"apple", "melon", "orange", "banana", "kiwi"};
  /*
      注意！ここで%以下がsとなっっていることに注意。
      つまりarray[0][1]のように、この二次元配列（単語も配列）のchar要素一つをとってくることはできません。
     %sはstring,すなわちchar配列を引数として撮り、最後の\0（null文字）となるまで読み込みを続けます。
      つまりここでは、最後の文字がaとなる単語を、a以降のみ読み込む必要があります。
      ここで最後がaなのは"banana"のみ。つまりbananaの5番目以降を読み込めばok。
   */
  printf("%s\n", array[3] + 5); // array[3]の先頭のcharから5個進める。
  printf("%s\n", &array[3][0] + 5);

  //以下は長くなりそうなので関数をわけます。
  // question6
  question6();
  question6_era();
  question8();

  return 0;
}

void question6() {
  for (int i = 2; i <= 1000; i++) {
    if (is_prime(i)) {
      printf("%d\n", i);
    }
  }
}

int is_prime(int num) {
  /*
     引数が総数かどうか判別する関数。素数であれば0を返します。素数ならば1。
     任意のintであるnumに対し、2からnum
     -1まで、numを割り切ることのできる数があるか調べる方法。
     2で割っているのはその数の半分より大きい約数は存在しないため。
     後述するエラトステネスの篩の方法を用いることで効率化できる（らしい）
   */
  for (int i = 2; i < num / 2; i++) {
    if (num % i == 0) {
      return 0;
    }
  }
  return 1;
}

void question6_era(int num) {
  /*ホームページのエラトステネスの方法を使ったもの。
     参考：http://mathtrain.jp/eratosthenes
     若干ホームページの記述と異なりますが、大まかな流れとしては、
     1.　prime_array[1000]というint配列を作る。
     2.　int配列の要素を全て1に（initialize）
     3.　prime[i]が0ではないならば、これの倍数となる数はi*j-1（jは任意の数）番目に存在するので、1000以下のi*j-1番目の数全ての値を0に変える。
     4.　最後にこの配列を最初から見ていき、i番目の値が1ならばすなわち1-1000の配列のi番目の数は素数になるとわかるので、iをprintfする。
   */
  int i, j;
  int prime_array[1000];

  for (i = 0; i < 1000; i++) {
    prime_array[i] = 1;
  }
  prime_array[0] = 0;

  for (i = 2; i * i < 1000; i++) {
    for (j = 2; i * j <= 1000; j++) {
      if (prime_array[i * j - 1] == 1)
        prime_array[i * j - 1] = 0;
    }
  }
  for (i = 0; i < 1000; i++) {
    if (prime_array[i] == 1)
      printf("%d\n", i + 1);
  }
}

void question8() {
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
