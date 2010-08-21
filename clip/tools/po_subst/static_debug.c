void static
debug(int level, const char *fmt, ...)
{
   va_list ap;

   if (level > verbose)
      return;

   va_start(ap, fmt);
   fprintf(stderr, "%s: debug %d: ", prgname, level);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   fflush(stderr);

   va_end(ap);
}
