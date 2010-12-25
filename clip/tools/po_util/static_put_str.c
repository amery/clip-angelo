static int
put_str(FILE * out, char *str)
{
   int num = 0, ch;

   for (; (ch = *str); ++str)
   {
      if (ch == '\n')
      {
	 num = 0;
	 if (str[1] == 0)
	 {
	    raw_put_str(out, "\\n");
	    return 0;
	 }
      }
      else if (num > max_num && (ch == ' ' || ch == '\t'))
      {
	 if (raw_put_str(out, "\"\n\""))
	    return -1;
	 num = 0;
      }
      else
	 num++;
      if (put_char(out, ch))
	 return -1;
   }
   return 0;
}
