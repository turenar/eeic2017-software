/*教科書にのっているstrlen関数のコード。これだけではコンパイルしません。mainからこの関数を呼ぶ必要があります。*/

int strlen1(char *s){
  char *p = s;

  while (*p != '\0'){
    p++;
  }
  return p - s;
}


