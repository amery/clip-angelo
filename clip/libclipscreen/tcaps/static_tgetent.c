static int
tgetent(char *bp, char *name, int beg)
{
   int j;

   int c;

   int tf = -1;

   char *cp = 0;

   char ibuf[TBUFSIZE];

   int i = 0;

   int cnt = 0;

   if (beg >= fnum)
      return 0;

   tbuf = bp;

   for (j = beg; j < fnum; ++j)
   {
      char *termcap = fnames[j];

      tf = open(termcap, 0);
      if (tf < 0)
	 continue;
      i = 0;
      cnt = 0;
      for (;;)
      {
	 cp = bp;
	 for (;;)
	 {
	    if (i == cnt)
	    {
	       cnt = read(tf, ibuf, TBUFSIZE);
	       if (cnt <= 0)
		  goto nextf;
	       i = 0;
	    }
	    c = ibuf[i++];
	    if (c == '\n')
	    {
	       if (cp > bp && cp[-1] == '\\')
	       {
		  cp--;
		  continue;
	       }
	       break;
	    }
	    if (cp >= bp + TBUFSIZE)
	    {
	       err("Termcap entry too long");
	       break;
	    }
	    else
	       *cp++ = c;
	 }
	 *cp = 0;

	 /* The real work for the match. */
	 if (tnamatch(name))
	 {
	    ++match_count;
	    close(tf);
	    return tnchktc(name, j);
	 }
      }
    nextf:
      close(tf);
   }
   return -1;
}
