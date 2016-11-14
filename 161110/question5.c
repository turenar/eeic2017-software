#include <stdio.h>

int main() {
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

  return 0;
}
