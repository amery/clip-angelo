static char *
my_index(str, chr)
	const char *str;

	int       chr;
{
   while (*str)
    {
       if (*str == chr)
	  return (char *) str;
       str++;
    }
   return 0;
}
