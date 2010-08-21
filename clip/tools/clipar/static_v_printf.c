static void
v_printf(int level, const char *fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   if (level > verbose_level)
      return;
   vfprintf(stdout, fmt, ap);
   va_end(ap);
}
