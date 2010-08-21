void
printString(FILE * stream, const char *str)
{
   const unsigned char *s;

   fputc('\"', stream);
   for (s = (unsigned char *) str; *s; s++)
      switch (*s)
      {
      case '"':
	 fputc('\\', stream);
	 fputc('"', stream);
	 break;
      case '\n':
	 fputc('\\', stream);
	 fputc('n', stream);
	 break;
      case '\t':
	 fputc('\\', stream);
	 fputc('t', stream);
	 break;
      case '\v':
	 fputc('\\', stream);
	 fputc('v', stream);
	 break;
      case '\b':
	 fputc('\\', stream);
	 fputc('b', stream);
	 break;
      case '\r':
	 fputc('\\', stream);
	 fputc('r', stream);
	 break;
      case '\f':
	 fputc('\\', stream);
	 fputc('f', stream);
	 break;
      case '\\':
	 fputc('\\', stream);
	 fputc('\\', stream);
	 break;

      default:
	 if (*s >= 32)
	    fputc(*s, stream);
	 else
	    fprintf(stream, "\\%03o", *s);
	 break;
      }
   fputc('\"', stream);
}
