static void
err(char *fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   vsnprintf(errbuf, errbuflen, fmt, ap);
   errcode = 1;

   va_end(ap);
}
