static int
mem_error(char *errbuf, int errbuflen, int line, const char *fmt, ...)
{
   int l;

   va_list ap;

   va_start(ap, fmt);
   snprintf(errbuf, errbuflen, "MEM: %s, line %d: ", __FILE__, line);
   l = strlen(errbuf);
   vsnprintf(errbuf + l, errbuflen - l, fmt, ap);
   va_end(ap);
   return 1;
}
