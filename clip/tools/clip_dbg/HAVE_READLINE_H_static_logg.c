static void
logg(int level, const char *fmt, ...)
{
   va_list ap;

   if (level > log_level)
      return;

   va_start(ap, fmt);
   vfprintf(stderr, fmt, ap);
   va_end(ap);
   fputc('\n', stderr);
   fflush(stderr);
}
