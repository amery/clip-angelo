/* get host inetrnet address and store it in ina */
static int
tcp_host_addr(const char *host, struct in_addr *ina)
{
   int ret = -1;

   if (host != NULL && *host != '\0')
   {
      /* gethostbyname in WinSock doesn't work with address in
         standard numbers-and-dots notation so check for n&d first */
      if (inet_aton(host, ina) != 0)
	 ret = 0;
      else
      {
	 struct hostent *hp = gethostbyname(host);

	 if (hp && hp->h_addrtype == AF_INET && hp->h_addr_list != NULL && hp->h_addr_list[0] != NULL)
	 {
	    memcpy(ina, hp->h_addr_list[0], hp->h_length);
	    ret = 0;
	 }
      }
   }

   if (ret == -1)
      ina->s_addr = INADDR_NONE;
   else if (ina->s_addr == INADDR_NONE)
      ret = -1;

   return ret;
}
