static int
tnamatch(char *np)
{
   register char *Np, *Bp;

   Bp = tbuf;
   if (*Bp == '#')
      return 0;
   for (;;)
   {
      for (Np = np; *Np && *Bp == *Np; Bp++, Np++)
	 continue;
      if (*Np == 0 && (*Bp == '|' || *Bp == ':' || *Bp == 0))
	 return (1);
      while (*Bp && *Bp != ':' && *Bp != '|')
	 Bp++;
      if (*Bp == 0 || *Bp == ':')
	 return (0);
      Bp++;
   }
}
