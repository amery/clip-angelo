char     *
strsuff(const char *str, const char *suff)
{
   const char *e;

   int       l = strlen(str);

   int       ls = strlen(suff);

   for (e = str + l - ls; e >= str; e--)
      if (!memcmp(e, suff, ls))
	 return (char *) e;
   return 0;
}
