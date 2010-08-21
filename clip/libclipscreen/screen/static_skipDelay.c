static char *
skipDelay(char *cp)
{
   while (*cp >= '0' && *cp <= '9')
      ++cp;
   if (*cp == '.')
   {
      ++cp;
      while (*cp >= '0' && *cp <= '9')
	 ++cp;
   }
   if (*cp == '*')
      ++cp;
   return (cp);
}
