/* get name of host pointed by host */
static const char *
tcp_host_name(const char *host)
{
   struct in_addr ina;

   const char *ret = "";

   if (host != NULL && *host != '\0')
   {
      struct hostent *hp;

      if (inet_aton(host, &ina) != 0)
	 hp = gethostbyaddr((const char *) &ina, sizeof(struct in_addr), AF_INET);
      else
	 hp = gethostbyname(host);

      if (hp && hp->h_addrtype == AF_INET)
	 ret = hp->h_name;
   }
   return ret;
}
