/*strcpyのコードです。これも同じくこれだけではコンパイルしないのでmainから呼ぶ必要があります。*/

void strcpy1(char *s, char *t)
{
  while ((*s =  *t) != '0') {
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
