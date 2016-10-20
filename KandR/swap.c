#include <stdio.h>

void swap(int *px, int *py);

int main () {
  int a = 2;
  int b = 3;

  printf("a is %d, b is %d\n.", a, b);//a is 2, b is 3
  swap(&a, &b);//&aはaへのポインタ。つまりa自体ではなくaのアドレスを与える/
  printf("a is %d, b is %d\n.", a, b);//a is 3, b is 2
  
  return 0;
}

/*@brief　swap関数はpx, pyと云うint型の値に対するポインタを受け取り、値の入れ替えを行う。*/
void swap(int *px, int *py) {
  int temp;//tempは一時的にpxの指す変数の値を保持するための変数。
  temp = *px;
  *px = *py;//ポインタpxの指している変数の値を、ポインタpyの指している値に変える。
  *py = temp;//ポインタpyの指してる変数の値を、temp(ポインタpxの指している値)に変える。
}

