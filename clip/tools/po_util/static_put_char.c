static int
put_char(FILE * out, int ch)
{
   switch (ch)
   {
   case '"':
      return raw_put_str(out, "\\\"");
   case '\n':
      return raw_put_str(out, "\\n\"\n\"");
   case '\r':
      return raw_put_str(out, "\\r");
   case '\v':
      return raw_put_str(out, "\\v");
   case '\t':
      return raw_put_str(out, "\\t");
   case '\\':
      return raw_put_str(out, "\\\\");
   default:
      if (ch >= 0 && ch < 32)
      {
	 char oct[5];

	 sprintf(oct, "\\03%o", ch);
	 return raw_put_str(out, oct);
      }
      else
      {
	 if (fputc(ch, out) == EOF)
	    return EOF;
      }
   }
   return 0;
}
