int
tgetnum(const char *id)
{
   register char *bp;

   register i, base;

   char name[2];

   if (!tbuf)
      return 0;
   bp = tbuf;
   for (;;)
   {
      bp = tskip(bp);
      if (!bp[0] || !bp[1])
	 break;
      if (bp[0] == ':' || bp[1] == ':')
	 continue;
      name[0] = *bp++;
      name[1] = *bp++;
      if (id[0] != name[0] || id[1] != name[1])
	 continue;
      if (*bp == '@')
	 continue;

      if (*bp != '#')
	 continue;
      bp++;
      base = 10;
      if (*bp == '0')
	 base = 8;
      i = 0;
      while (*bp >= '0' && *bp <= '9')
	 i = i * base, i += *bp++ - '0';
      return i;
   }
   return 0;
}
