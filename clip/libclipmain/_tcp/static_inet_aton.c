static int
inet_aton(const char *host, struct in_addr *ina)
{
   in_addr_t a;

   a = inet_addr(host);
   if (((int) a) == -1)
      return -1;

   ina->s_addr = a;

   return 0;
}
