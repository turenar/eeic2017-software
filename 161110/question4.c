#include <stdio.h>

int main() {
  // question4
  printf("%c\n", ("hello"[1] - 1)); // output:d

  /*
     "hello"[1]
     は"hello"というchar配列の2番目（注）の要素、つまり'e'を返します。
     char'd'から-1しているので,ASCIIテーブルでcharの一つまえ、つまり小文字のdが帰ってきます。
     ASCII表: http://www9.plala.or.jp/sgwr-t/c_sub/ascii.html
     （注）Cは0-indexing、つまり0から数え上げが始まります。うっかり最初の要素だと思いわないように注意。
   */
  return 0;
}
