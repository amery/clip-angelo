static int
find_macro(char *s, char *e, char **beg, char **end, char **next, int *dot)
{
   *dot = 0;
   if (s >= e)
      return 0;
   for (; s < e; ++s)
      if (*s == '&')
      {
	 ++s;
	 if (s >= e)
	    return 0;
	 *beg = s;
	 if (*s == '.')
	    *dot = 1;
	 if (*s == '\\')
	 {
	    ++s;
	    if (s >= e)
	       return 0;
	    switch (*s)
	    {
	    case 'x':
	    case 'X':
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	       goto cont;
	    }
	    ++s;
	    *end = s;
	    *next = s;
	    return 1;
	 }
	 else
	 {
	    if (*s == '.')
	       *dot = 1;

	  cont:

	    for (; s <= e; ++s)
	       if (s == e || !(isalnum(*s) || *s == '_'))
	       {
		  *end = s;
		  if (*s == '.')
		  {
		     ++s;
		     *dot = 1;
		  }
		  *next = s;
		  return 1;
	       }
	 }
	 *end = s;
	 ++s;
	 *next = s;
      }
   return 0;
}
