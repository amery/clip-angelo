static int
get_int32(char **buf, long *buflen, long *resp)
{
   int res;

   if (*buflen < 4)
      return 0;

   res = (ntohl(*((int *) *buf)));
   *resp = (long) res;

   (*buf) += sizeof(res);
   (*buflen) -= sizeof(res);

   return 4;
}
