static int
tnchktc(char *name, int beg)
{
   register char *p, *q;

   char tcname[16];		/* name of similar terminal */

   char tcbuf[TBUFSIZE];

   char *holdtbuf = tbuf;

   int l;

   p = tbuf + strlen(tbuf) - 2;	/* before the last colon */
   while (*--p != ':')
      if (p < tbuf)
      {
	 err("Bad termcap entry");
	 return 0;
      }
   p++;
   /* p now points to beginning of last field */
   if (p[0] != 't' || p[1] != 'c')
      return 1;
   strcpy(tcname, p + 3);
   q = tcname;
   while (*q && *q != ':')
      q++;
   *q = 0;
   if (++hopcount > MAXHOP)
   {
      err("Infinite tc= loop");
      return 0;
   }

   if (!strcmp(tcname, name))
      ++beg;
   if (tgetent(tcbuf, tcname, beg) <= 0)
   {
      hopcount = 0;		/* unwind recursion */
      return 1;
   }
   for (q = tcbuf; *q != ':'; q++)
      ;
   l = p - holdtbuf + strlen(q);
   if (l > TBUFSIZE)
   {
      err("Termcap entry too long\n");
      q[TBUFSIZE - (p - tbuf)] = 0;
   }
   strcpy(p, q + 1);
   tbuf = holdtbuf;
   hopcount = 0;		/* unwind recursion */

   return 1;
}
