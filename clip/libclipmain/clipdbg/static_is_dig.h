static int
is_dig(const char *s, int l)
{
   int       i;

   for (i = 0; i < l; i++)
      if (!isdigit(s[i]))
	 return 0;
   return 1;
}
