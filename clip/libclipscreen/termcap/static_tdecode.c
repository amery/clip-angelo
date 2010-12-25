static char *
tdecode(char *str, char **area)
{
   register char *cp;

   register int c;

   register char *dp;

   int i;

   cp = *area;
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
      *cp++ = c;
   }
   *cp++ = 0;
   str = *area;
   *area = cp;
   return (str);
}
