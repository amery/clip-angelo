static int
get_str(char **buf, long *buflen, char **strp, long *lenp)
{
   long l;

   if (get_int32(buf, buflen, &l) != 4)
      return 0;
   if (*buflen < l)
      return 0;

   *lenp = l;
   *strp = (char *) calloc(1 /**strp*/ , l + 1);
   memcpy(*strp, *buf, l);
   (*strp)[l] = 0;

   (*buf) += l;
   (*buflen) -= l;

   return l + 4;
}
