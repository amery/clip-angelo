int
clip_UDPRECVFROM(ClipMachine * ClipMachineMemory)
// UDPRECVFROM( nCF, @cIPadr, @nPort, @cMsg, [nLen], [nTimeout] )
{
   int fd = _clip_parni(ClipMachineMemory, 1), lenbuf, sln;

   C_FILE *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   char *msg = _clip_parcl(ClipMachineMemory, 4, &lenbuf);

   long len = _clip_parnl(ClipMachineMemory, 5);

   int timeout = _clip_parni(ClipMachineMemory, 6);

   int ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   char ipaddr[INET_ADDRSTRLEN];

   struct sockaddr_in sin;

   struct timeval tv;

   fd_set set;

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
   {
      if (_clip_parinfo(ClipMachineMemory, 5) != NUMERIC_type_of_ClipVarType || len > lenbuf)
	 len = lenbuf;

      if (_clip_parinfo(ClipMachineMemory, 6) != NUMERIC_type_of_ClipVarType)
	 timeout = cf->timeout_of_C_FILE;

      if (timeout >= 0)
      {
	 FD_ZERO(&set);
	 FD_SET(cf->fileno_of_C_FILE, &set);
	 tv.tv_sec = timeout / 1000;
	 tv.tv_usec = (timeout % 1000) * 1000;
      }

      memset((void *) &sin, 0, sizeof(sin));
      sin.sin_family = PF_INET;
      sln = sizeof(sin);
      if (timeout < 0 || select(cf->fileno_of_C_FILE + 1, &set, NULL, NULL, &tv) > 0)
	 ret = recvfrom(cf->fileno_of_C_FILE, msg, len, 0, (struct sockaddr *) &sin, (socklen_t *) (&sln));
      else
#ifdef OS_MINGW
	 *err = EAGAIN;
#else
	 *err = ETIMEDOUT;
#endif

      if (ret >= 0)
      {
#ifdef _WIN32
	 _clip_storc(ClipMachineMemory, inet_ntoa(*(struct in_addr *) ipaddr), 2, 0);
	 _clip_storni(ClipMachineMemory, ntohs(sin.sin_port), 3, 0);
#else
	 if (inet_ntop(PF_INET, &sin.sin_addr.s_addr, ipaddr, INET_ADDRSTRLEN) == NULL)
	    ret = -1;
	 else
	 {
	    _clip_storc(ClipMachineMemory, ipaddr, 2, 0);
	    _clip_storni(ClipMachineMemory, ntohs(sin.sin_port), 3, 0);
	 }
#endif
      }

      *err = ret == -1 ? errno : 0;
   }

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
