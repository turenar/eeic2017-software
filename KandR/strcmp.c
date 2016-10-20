/*教科書のstrcmpコードです。mainから読んでください。同様にmainから読んでください。*/

int strcmp1(char *s, char *t)
{
  for ( ; *s == *t; s++, t++)
    if (*s == '\0')
      return 0;
  return *s - *t;
}
