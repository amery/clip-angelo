int
clip_TCPGETPEERNAME(ClipMachine * ClipMachineMemory)
/* Get the address of the peer to which a socket is connected.
   using : tcpGetPeerName( sock, @cIPadr ) ->  0 - OK  | -1 == error
*/
{
   int ret = -1, sln;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   char ipaddr[INET_ADDRSTRLEN];

   struct sockaddr_in sin;

   _clip_retnl(ClipMachineMemory, -1);

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
   {
      *err = EBADF;
   }
   else
   {
      sln = sizeof(sin);
      ret = getpeername(cf->fileno_of_C_FILE, (struct sockaddr *) &sin, (socklen_t *) (&sln));

      *err = ret < 0 ? errno : 0;

      if (ret >= 0)
      {
#ifdef _WIN32
	 _clip_storc(ClipMachineMemory, inet_ntoa(*(struct in_addr *) ipaddr), 2, 0);
#else
	 if (inet_ntop(PF_INET, &sin.sin_addr.s_addr, ipaddr, INET_ADDRSTRLEN) == NULL)
	    ret = -1;
	 else
	    _clip_storc(ClipMachineMemory, ipaddr, 2, 0);
#endif
	 _clip_storni(ClipMachineMemory, ntohs(sin.sin_port), 3, 0);
      }
      else
	 _clip_storni(ClipMachineMemory, -1, 3, 0);

   }

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
