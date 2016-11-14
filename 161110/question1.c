#include <stdio.h>

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
  return 0;
}
