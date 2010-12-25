/*
   // Routine to perform cursor addressing.
   // CM is a string containing printf type escapes to allow
   // cursor addressing.  We start out ready to print the destination
   // line, and switch each time we print row or column.
   // The following escapes are defined for substituting row/column:
   //
   //      %d      as in printf
   //      %2      like %2d
   //      %3      like %3d
   //      %.      gives %c hacking special case characters
   //      %+x     like %c but adding x first
   //
   //      The codes below affect the state but don't use up a value.
   //
   //      %>xy    if value > x add y
   //      %r      reverses row/column
   //      %i      increments row/column (for one origin indexing)
   //      %%      gives %
   //      %B      BCD (2 decimal digits encoded in one byte)
   //      %D      Delta Data (backwards bcd)
   //
   // all other characters are ``self-inserting''.
 */

char *
tgoto(char *CM, int destcol, int destline)
{
   char *cp, *dp;

   int c, which, oncol;

   static char result[MAXRETURNSIZE];

#ifdef DEBUG
   printf("CM='%s'\n", CM);
#endif
   cp = CM;
   if (!cp)
    toohard:return ("bad capgoto");
   dp = result;
   oncol = 0;
   which = destline;
   while ((c = *cp++))
   {
      if (c != '%')
      {
	 *dp++ = c;
	 continue;
      }
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
       two:*dp++ = which / 10 | '0';
       one:*dp++ = which % 10 | '0';
       swap:oncol = 1 - oncol;
       setwhich:which = oncol ? destcol : destline;
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
	 return (result);
      default:
	 goto toohard;
      }
   }
   *dp = 0;
   return (result);
}
