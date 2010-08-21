static int
put_str(FILE * out, char *str)
{
   for (; *str; ++str)
      if (put_char(out, *str) == EOF)
	 return -1;
   return 0;
}
