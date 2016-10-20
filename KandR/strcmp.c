/*教科書のstrcmpコードです。同様にmainから呼んでください。*/

int strcmp1(char *s, char *t)
{
  for ( ; *s == *t; s++, t++) {
    if (*s == '\0') {
      return 0;
    }
  }
  return *s - *t;
}
