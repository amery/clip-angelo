void
fprintfOffs(FILE * file, int offs, const char *format, ...)
{
   va_list   ap;

   while (offs-- > 0)
      fputc('\t', file);
   va_start(ap, format);
   vfprintf(file, format, ap);
   va_end(ap);
}
