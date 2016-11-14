#include <stdio.h>

int main() {
  // question3
  int *a, b = 10;
  a = &b;
  printf("%d\n", *a); // output: 10
  //ちなみにもちろん以下もオッケーですが問題の趣旨的に多分ポインタを使う方が正しいかと。
  printf("%d\n", *a); // output: 10
  return 0;
}
