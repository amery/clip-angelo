static void
error(const char *fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   fprintf(stderr, "clipar: ");
   vfprintf(stderr, fmt, ap);
   fputc('\n', stderr);
   va_end(ap);
   if (tmp_file[0])
      unlink(tmp_file);
   exit(3);
}
