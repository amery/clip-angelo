int
haveMacro(char *str)
{
   char     *s;

   for (s = str; *s; ++s)
      if (*s == '&')
	 return 1;
   return 0;
}
