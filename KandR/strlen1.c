/*教科書にのっているstrlen関数のコード。これだけではコンパイルしません。mainからこの関数を呼ぶ必要があります。*/

int strlen1(char *s){
  char *p = s;

  while (*p != '\0'){//文字列の最後になるまでp++をインクリメントする。つまりの次の文字を指ししめす。
    p++;
  }
  return p - s;//sで奨めれられた文字数、つまり文字列の長さが与えられる。
}

//別解。こちらの方が直感的に理解しやすいかもしれないです。
int strlen2(char *s){
  int n;

  for (n = 0; *s != '\0'; s++)
    n++;
  return n;
}


