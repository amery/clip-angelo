static int
tnchktc()
{
   register char *p, *q;

   char tcname[16];		/* name of similar terminal */

   char *tcbuf;

   char *holdtbuf = tbuf;

   int l;

   p = tbuf + strlen(tbuf) - 2;	/* before the last colon */
   while (*--p != ':')
      if (p < tbuf)
      {
	 errexit("Bad termcap entry");
	 return (0);
      }
   p++;
   /* p now points to beginning of last field */
   if (p[0] != 't' || p[1] != 'c')
      return (1);
   strcpy(tcname, p + 3);
   q = tcname;
   while (*q && *q != ':')
      q++;
   *q = 0;
   if (++hopcount > MAXHOP)
   {
      errexit("Infinite tc= loop");
      return (0);
   }
   tcbuf = (char *) malloc(BUFSIZE);
   if (!_tgetent(tcbuf, tcname, tcapfile))
   {
      hopcount = 0;		/* unwind recursion */
      free(tcbuf);
      return (0);
   }
   for (q = tcbuf; *q != ':'; q++);
   l = p - holdtbuf + strlen(q);
   if (l > BUFSIZE)
   {
      errexit("Termcap entry too long\n");
      q[BUFSIZE - (p - tbuf)] = 0;
   }
   strcpy(p, q + 1);
   tbuf = holdtbuf;
   hopcount = 0;		/* unwind recursion */
   free(tcbuf);
   return (1);
}
