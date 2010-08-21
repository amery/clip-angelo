int
loc_cmp(DbfLocale * loc, unsigned char *s1, unsigned char *s2, int len)
{
   int r = 0;

   unsigned int ch;

   unsigned char *ss1, *ss2;

   unsigned char *e = s1 + len;

   int ch1, ch2;

   for (ss1 = s1, ss2 = s2; ss1 < e && !r; ss1++, ss2++)
   {
      ch = *ss1;
      if (ch >> 7)
	 ch = loc->read[ch & 0x7F];
      ch1 = _clip_cmptbl[ch];
      ch2 = _clip_cmptbl[*ss2];
      r = ch1 - ch2;
   }
   return r;
}
