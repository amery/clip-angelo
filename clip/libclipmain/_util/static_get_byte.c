static int
get_byte(char **buf, long *buflen, int *resp)
{
   if (*buflen < 1)
      return 0;
   *resp = *((unsigned char *) *buf);

   (*buf)++;
   (*buflen)--;

   return 1;
}
