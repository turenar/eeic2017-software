#include <stdio.h>

void question6();
int is_prime(int num);
void question6_era();

int main() {
  //方法1:ひたすらある数字より小さい数がその数の役数ではないか調べる。
  question6();
  //方法2:エラトステネスの篩を用いる。
  question6_era();

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
     (もっというと素数判定では平方根以下のみ考えればいいのですが、int程度の桁では/2のほうが速いことが多いと思う……)
     後述するエラトステネスの篩の方法を用いることで効率化できる（らしい）
   */
  for (int i = 2; i < num / 2; i++) {
    if (num % i == 0) {
      return 0;
    }
  }
  return 1;
}

void question6_era() {
  /*ホームページのエラトステネスの方法を使ったもの。
     参考：http://mathtrain.jp/eratosthenes
     若干ホームページの記述と異なりますが、大まかな流れとしては、
     1.　prime_array[1001]というint配列を作る。
           ←prime_array[2]〜prime_array[1000]をそのまま[2〜1000が素数かどうか]に対応させるため
            別に[1000]で作って3.の処理をうまく変えるのもおっけーです。
     2.　int配列の要素を全て1に（initialize）
     3.　prime[i]が0ではないならば、これの倍数となる数はi*j（jは任意の数）番目に存在するので、1000以下のi*j番目の数全ての値を0に変える。
     4.　最後にこの配列を最初から見ていき、i番目の値が1ならばiは素数であるとわかるので、iをprintfする。

     ぶっちゃけ、配列の大きさを+1したり3.の処理をうまくやるのは間違いのもとなので、
     指定がない限りエラトステネスの篩は使わないほうが吉です。
     たとえ愚直でも、動けばいいんです。

     ちなみに下のコードは3.をうまくやるタイプのコードになります。
   */
  int i, j;
  int prime_array[1000];

  for (i = 0; i < 1000; i++) {
    prime_array[i] = 1;
  }
  prime_array[0] = 0;

  for (i = 2; i * i < 1000; i++) {
    for (j = 2; i * j <= 1000; j++) {
      if (prime_array[i * j - 1] == 1){
        prime_array[i * j - 1] = 0;
      }
    }
  }
  for (i = 0; i < 1000; i++) {
    if (prime_array[i] == 1){
      printf("%d\n", i + 1);
    }
  }
}
