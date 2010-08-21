static int
raw_put_str(FILE * out, char *str)
{
   for (; *str; ++str)
      if (fputc(*str, out) == EOF)
	 return EOF;
   return 0;
}
