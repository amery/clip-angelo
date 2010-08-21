int
clip_UDPBIND(ClipMachine * ClipMachineMemory)
{
   int fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   char *ipaddr = _clip_parc(ClipMachineMemory, 2);

   int port = _clip_parni(ClipMachineMemory, 3);

   int ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   struct sockaddr_in sin;

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
   {
      memset((void *) &sin, 0, sizeof(sin));
      sin.sin_family = PF_INET;
      sin.sin_addr.s_addr = inet_addr(ipaddr);
      sin.sin_port = htons(port);
      ret = bind(cf->fileno_of_C_FILE, (struct sockaddr *) &sin, sizeof(sin));
      *err = ret == -1 ? errno : 0;
   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
