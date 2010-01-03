static int
put_char(FILE * out, int ch)
{
   switch (ch)
    {
    case '"':
       return fputs("\\\"", out);
    case '\n':
       return fputs("\\n\"\n\"", out);
    case '\r':
       return fputs("\\r", out);
    case '\v':
       return fputs("\\v", out);
    case '\t':
       return fputs("\\t", out);
    default:
       if (ch >= 0 && ch < 32)
	{
	   char      oct[5];

	   sprintf(oct, "\\03%o", ch);
	   return fputs(oct, out);
	}
       else
	{
	   if (fputc(ch, out) == EOF)
	      return EOF;
	}
    }
   return 0;
}
