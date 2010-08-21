int
tgetflag(const char *id)
{
   register char *bp;

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

      switch (*bp)
      {
      case '#':
      default:
	 continue;
      case ':':
	 return 1;
      case 0:
	 return 0;
      }
   }
   return 0;
}
