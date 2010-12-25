static char *
branchto(char *cp, int to)
{
   int c, level;

   level = 0;
   while ((c = *cp++))
   {
      if (c == '%')
      {
	 if ((c = *cp++) == to || c == ';')
	 {
	    if (level == 0)
	    {
	       return cp;
	    }
	 }
	 if (c == '?')
	    level++;
	 if (c == ';')
	    level--;
      }
   }
   return ("no matching ENDIF");
}
