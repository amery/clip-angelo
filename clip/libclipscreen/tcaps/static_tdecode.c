static int
tdecode(char *bp, Terminfo * info)
{
   int c;

   char *dp;

   int i;

   char *str;

   int retpos;

   /* skip the delay */
#if 0
   while (isdigit(*bp))
      ++bp;
#endif

   str = bp;
   retpos = info->bufpos;
   while ((c = *str++) && c != ':')
   {
      switch (c)
      {

      case '^':
	 c = *str++ & 037;
	 break;

      case '\\':
	 dp = "E\033^^\\\\::n\nr\rt\tb\bf\f";
	 c = *str++;
       nextc:
	 if (*dp++ == c)
	 {
	    c = *dp++;
	    break;
	 }
	 dp++;
	 if (*dp)
	    goto nextc;
	 if (c >= '0' && c <= '9')
	 {
	    c -= '0', i = 2;
	    do
	       c <<= 3, c |= *str++ - '0';
	    while (--i && *str >= '0' && *str <= '9');
	 }
	 break;
      }
      place_buf(info, 1);
      info->buf[info->bufpos++] = c;
   }

   if (info->bufpos == retpos)
      return -1;

   place_buf(info, 1);
   info->buf[info->bufpos++] = 0;

   return retpos;
}
