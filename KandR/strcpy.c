/*strcpyのコードです。これも同じくこれだけではコンパイルしないのでmainから呼ぶ必要があります。*/

void strcpy1(char *s, char *t)
{
  while ((*s =  *t) != '\0') {
    s++;
    t++;
  }
}

//別解（マスターすべきとは書いてありますが、個人的には初見で理解しにくいので、はじめのうちは避ける方がベターな気がします…
void strcpy2(char *s, char *t){
  while (*s++ = *t++) {
    ; // do nothing
  }
}

//別解2というか、参考実装です。strcpy1をもっと冗長に書いた感じのコード
void strcpy3(char *s, char *t) {
  while (*s != '\0') {
    *s = *t;
    s++;
    t++;
  }
  *s = '\0'; // この行がなぜstrcpy3では必要かを考えてみてください
}
