int
clip_UDPSENDTO(ClipMachine * ClipMachineMemory)
// UDPSENDTO( nCF, cIPaddr, nPort, cMsg, [nLen], [nTimeout] )
{
   int fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   char *ipaddr = _clip_parc(ClipMachineMemory, 2);

   int port = _clip_parni(ClipMachineMemory, 3), lenbuf;

   char *msg = _clip_parcl(ClipMachineMemory, 4, &lenbuf);

   long len = _clip_parnl(ClipMachineMemory, 5);

   int timeout = _clip_parni(ClipMachineMemory, 6);

   int ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   struct sockaddr_in sin;

   struct timeval tv;

   fd_set set;

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
   {
      if (_clip_parinfo(ClipMachineMemory, 5) != NUMERIC_type_of_ClipVarType)
	 len = lenbuf;
      if (len > lenbuf)
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
      sin.sin_addr.s_addr = inet_addr(ipaddr);
      sin.sin_port = htons(port);

      if (timeout < 0 || select(cf->fileno_of_C_FILE + 1, NULL, &set, NULL, &tv) > 0)
      {
	 ret = sendto(cf->fileno_of_C_FILE, msg, len, 0, (struct sockaddr *) &sin, sizeof(sin));
	 *err = ret == -1 ? errno : 0;
      }
      else
#ifdef OS_MINGW
	 *err = EAGAIN;
#else
	 *err = ETIMEDOUT;
#endif

   }

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
