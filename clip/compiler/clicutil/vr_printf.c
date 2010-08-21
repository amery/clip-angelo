int
vr_printf(int level, const char *fmt, ...)
{
   va_list ap;

   int ret = 0;

   if (level > verbose)
      return 0;

   for (; v_neednl; v_neednl--)
      fputc('\n', stdout);

   va_start(ap, fmt);
   ret = vfprintf(stdout, fmt, ap);
   va_end(ap);
   fflush(stdout);

   return ret;
}
