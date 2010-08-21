/*
 * Skip spaces and read U+1234 or return -1 for error.
 * Return first non-read position in *p0 (unchanged on error).
 */
static int
getunicode(char **p0)
{
   char *p = *p0;

   while (*p == ' ' || *p == '\t')
      p++;
   if (*p != 'U' || p[1] != '+' || !isxdigit(p[2]) || !isxdigit(p[3]) || !isxdigit(p[4]) || !isxdigit(p[5]) || isxdigit(p[6]))
      return -1;
   *p0 = p + 6;
   return strtol(p + 2, 0, 16);
}
