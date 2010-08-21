char *
tgetstr(char *id, char **area)
{
   register char *bp;

   char name[2];

   if (!tbuf)
      return NULL;
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
      case ':':
      case 0:
	 break;
      default:
	 if (*bp != '=')
	    continue;
	 bp++;
	 return tdecode(bp, area);
      }
   }
   return NULL;
}
