static int
sql_cmp(SQLLocale * loc, unsigned char *s1, unsigned char *s2, int len)
{
   int r = 0;

   unsigned char *ss1, *ss2;

   unsigned char *e = s1 + len;

   int ch1, ch2;

   for (ss1 = s1, ss2 = s2; ss1 < e && !r; ss1++, ss2++)
   {
      ch1 = _clip_cmptbl[*ss1];
      ch2 = _clip_cmptbl[*ss2];
      r = ch1 - ch2;
   }
   return r;
}
