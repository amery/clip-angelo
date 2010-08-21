void
scr_tgoto(char *CM, int destcol, int destline, char *buf, int buflen)
{
   char *cp, *dp, *ep;

   int c, which, oncol;

#ifdef DEBUG
   printf("CM='%s'\n", CM);
#endif
   cp = CM;
   if (!cp)
    toohard:
      return;
   dp = buf;
   ep = dp + buflen - 1;
   oncol = 0;
   which = destline;
   while (dp < ep && (c = *cp++))
   {
      switch (c)
      {
      default:
	 *dp++ = c;
	 continue;

      case '%':
	 switch (c = *cp++)
	 {
	 case 'n':
	    destcol ^= 0140;
	    destline ^= 0140;
	    goto setwhich;
	 case 'd':
	    if (which < 10)
	       goto one;
	    if (which < 100)
	       goto two;
	    /* fall into... */
	 case '3':
	    *dp++ = (which / 100) | '0';
	    which %= 100;
	    /* fall into... */
	 case '2':
	  two:
	    *dp++ = which / 10 | '0';
	  one:
	    *dp++ = which % 10 | '0';
	  swap:
	    oncol = 1 - oncol;
	  setwhich:
	    which = oncol ? destcol : destline;
	    continue;
	 case '>':
	    if (which > *cp++)
	       which += *cp++;
	    else
	       cp++;
	    continue;
	 case '+':
	    which += *cp++;
	    /* fall into... */
	 case '.':
	    *dp++ = which;
	    goto swap;
	 case 'r':
	    oncol = 1;
	    goto setwhich;
	 case 'i':
	    destcol++;
	    destline++;
	    which++;
	    continue;
	 case '%':
	    *dp++ = c;
	    continue;
	 case '/':
	    c = *cp;
	    *dp++ = which / c | '0';
	    which %= *cp++;
	    continue;
	 case 'B':
	    which = (which / 10 << 4) + which % 10;
	    continue;
	 case 'D':
	    which = which - 2 * (which % 16);
	    continue;
	 case 'p':
	 case 'P':
	 case '\'':
	 case '{':		/* } */
	 case '?':
	    /* string is in tparm format... */
	    tparm(dp, cp - 2, destline, destcol);
	    return;
	 default:
	    goto toohard;
	 }
      }
   }
   *dp = 0;
}
