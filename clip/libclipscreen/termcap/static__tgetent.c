static int
_tgetent(char *bp, char *name, char *termcap)
{
   int c;

   int tf = -1;

   char *cp = envtermcap;

   char *ibuf;

   int i = 0;

   int cnt = 0;

   tbuf = bp;

   /* TERMCAP can have one of two things in it. It can be the */
   /* name of a file to use instead of /etc/termcap. In this */
   /* case it must start with a "/". Or it can be an entry to */
   /* use so we don't have to read the file. In this case it */
   /* has to already have the newlines crunched out. */
   if (cp && *cp)
   {
      envtermcap = "";
      tbuf = cp;
      c = tnamatch(name);
      tbuf = bp;
      if (c)
      {
	 strcpy(bp, cp);
	 return (tnchktc());
      }
   }
   if (tf < 0)
      tf = open(termcap, 0);
   if (tf < 0)
      return (0);
   ibuf = (char *) malloc(BUFSIZE);
   i = 0;
   cnt = 0;
   for (;;)
   {
      cp = bp;
      for (;;)
      {
	 if (i == cnt)
	 {
	    cnt = read(tf, ibuf, BUFSIZE);
	    if (cnt <= 0)
	    {
	       close(tf);
	       free(ibuf);
	       return (0);
	    }
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
	 if (cp >= bp + BUFSIZE)
	 {
	    errexit("Termcap entry too long");
	    break;
	 }
	 else
	    *cp++ = c;
      }
      *cp = 0;

      /* The real work for the match. */
      if (tnamatch(name))
      {
	 close(tf);
	 free(ibuf);
	 return (tnchktc());
      }
   }
   return -1;
}
